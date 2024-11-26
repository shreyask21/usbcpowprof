; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Main Binder Code
; ----------------------------------------------


;===============================================
; Legend:
;		i_ -> Immediate value
;		r_ -> Register defination
;		l_ -> Label
;		m_ -> Macro
;		s_ -> String
;===============================================

.cseg
.org 0x0000

jmp SETUP
.include "uart_out.inc"
.include "str_data.inc"
.include "common.inc"
.include "gpio_driver.inc"
.include "lcd_driver.inc"
.include "lcd_prnum.inc"
.include "i2c_driver.inc"
.include "buzzer_tone.inc"
.include "ina219_driver.inc"

SETUP:
	m_INIT_STACK
	;m_POWERSAVE
	m_INIT_LCD
	m_BUZZER 			3, 80
	m_BUZZER 			4, 80
	m_BUZZER 			5, 80
	m_INIT_INA219
	m_UART_INIT
	m_LCD_SEND_COMMAND	i_CMD_LCD_CLR
	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE1
	m_PRINT_STRING		s_SPLASH_STR_1
	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE2
	m_PRINT_STRING		s_SPLASH_STR_2
	m_DELAY_MS			2000

	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE1
	m_PRINT_STRING		s_SPLASH_STR_3
	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE2
	m_PRINT_STRING		s_SPLASH_STR_4
	m_DELAY_MS			2000
	m_BUZZER 			6, 50
	m_DELAY_MS			100
	m_BUZZER 			6, 50
	m_DELAY_MS			1000
	rjmp LOOP
	
LOOP:
	m_LCD_SEND_COMMAND	i_CMD_LCD_LINE1
	m_PRINT_VOLTAGE
	m_PRINT_CURRENT
rjmp LOOP
