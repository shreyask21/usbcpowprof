; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Macros for INA219 - TWI.
; ----------------------------------------------

.def r_INA219_LASTDIR = r20
.def r_INA219_DOWNSAMPLE = r21

.equ i_INA219_I2C_7bADDR	= 0x40
.equ i_INA219_CONF_REG		= 0x00
.equ i_INA219_CONF_RESET	= 0x8000
.equ i_INA219_VBUS_16V	= 0x0000
.equ i_INA219_VBUS_32V	= 0x2000
.equ i_INA219_VS_040mV	= 0x0000
.equ i_INA219_VS_080mV	= 0x0800
.equ i_INA219_VS_160mV	= 0x1000
.equ i_INA219_VS_320mV	= 0x1800
.equ i_INA219_CONTINOUS	= 0x0007
.equ i_INA219_12BIT_AVG16 = 0x05D8

.equ i_INA219_CAL_REG		= 0x05
.equ i_INA219_CAL_100mE		= 1675

.equ i_INA219_SHUNT_V_REG	= 0x01
.equ i_INA219_BUS_V_REG		= 0x02
.equ i_INA219_POWER_W_REG	= 0x03
.equ i_INA219_CURR_A_REG	= 0x04

.macro m_INIT_INA219
	rcall l_INIT_INA219
.endmacro

.macro m_PRINT_VOLTAGE
	rcall l_INA219_PR_VOLTAGE
.endmacro

.macro m_PRINT_CURRENT
	rcall l_INA219_PR_CURRENT
.endmacro

.macro m_INA219_WAIT_NEWSAMPLE
	rcall l_INA219_WAIT_NEWSAMPLE
.endmacro

l_INIT_INA219:
	m_INIT_I2C
	m_I2C_WRITE16 i_INA219_I2C_7bADDR,i_INA219_CONF_REG,i_INA219_CONF_RESET
	m_delay_ms 100
	m_I2C_WRITE16 i_INA219_I2C_7bADDR,i_INA219_CAL_REG,i_INA219_CAL_100mE
	m_I2C_WRITE16 i_INA219_I2C_7bADDR,i_INA219_CONF_REG,(i_INA219_VBUS_32V|i_INA219_VS_320mV|i_INA219_12BIT_AVG16|i_INA219_CONTINOUS)
	clr r_INA219_DOWNSAMPLE
ret

l_INA219_PR_VOLTAGE:
	push r_TWI_DH
	push r_TWI_DL
	
	l_INA219_WAIT_NEWSAMPLE:	
		;m_DELAY_MS 1 ;limit update rate
		m_I2C_READ16 i_INA219_I2C_7bADDR,i_INA219_BUS_V_REG
		sbrs r_TWI_DL,1
		rjmp l_INA219_WAIT_NEWSAMPLE
	; now we got the new sample
	; first 3 bits are for status, mask them and left justify actual ADC data
	andi r_TWI_DL, 0xF8			
	lsr r_TWI_DH
	ror r_TWI_DL

	m_UART_TRANSMIT_CRLF
	m_UART_TRANSMIT_CHAR
	push r_TWI_DL
	mov r_TWI_DL, r_TWI_DH
	m_UART_TRANSMIT_CHAR
	pop r_TWI_DL

	inc r_INA219_DOWNSAMPLE
	cpi r_INA219_DOWNSAMPLE, 0x0F
	breq l_INA219_PR_VOLTAGE_LCD
	rjmp l_INA219_PR_VOLTAGE_LCD_EXIT
l_INA219_PR_VOLTAGE_LCD:
	rcall l_LCD_PRINTu16
	m_PRINT_STRING s_mV
l_INA219_PR_VOLTAGE_LCD_EXIT:
	pop r_TWI_DL
	pop r_TWI_DH
ret

l_INA219_PR_CURRENT:
	push r_TWI_DH
	push r_TWI_DL
	m_I2C_READ16 i_INA219_I2C_7bADDR,i_INA219_CURR_A_REG
	

	m_UART_TRANSMIT_CHAR
	push r_TWI_DL
	mov r_TWI_DL, r_TWI_DH
	m_UART_TRANSMIT_CHAR
	pop r_TWI_DL

	bst r_TWI_DH, 7 ; store sign bit for later

	cpi r_INA219_DOWNSAMPLE, 0x0F
	breq l_INA219_PR_CURRENT_LCD
	rjmp l_INA219_PR_CURRENT_LCD_EXIT
l_INA219_PR_CURRENT_LCD:
	clr r_INA219_DOWNSAMPLE
	rcall l_LCD_PRINTs16
	m_PRINT_STRING s_mA
	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE2
	brtc l_PR_FW
	brts l_PR_BW
	l_PR_FW:
		sbrs r_INA219_LASTDIR, 0x00
		rcall l_UPDATE_FW
		ldi r_INA219_LASTDIR, 0x01
		rjmp l_INA219_PR_CURRENT_LCD_EXIT
	l_PR_BW:
		sbrc r_INA219_LASTDIR, 0x00
		rcall l_UPDATE_BW
		ldi r_INA219_LASTDIR, 0x00
		rjmp l_INA219_PR_CURRENT_LCD_EXIT
l_INA219_PR_CURRENT_LCD_EXIT:	
	pop r_TWI_DL
	pop r_TWI_DH
ret

l_UPDATE_FW:
	m_PRINT_STRING 		s_FOOTER_FW
	m_BUZZER 3, 100
	m_DELAY_MS 150
	m_BUZZER 5, 100
ret

l_UPDATE_BW:
	m_PRINT_STRING 		s_FOOTER_BW
	m_BUZZER 5, 100
	m_DELAY_MS 150
	m_BUZZER 3, 100
ret