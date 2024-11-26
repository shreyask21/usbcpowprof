; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Macros for LCD.
; ----------------------------------------------

; scrachpad reg for lcd operations
.def r_LCD_iTMP			= r16 
.def r_LCD_TMPL			= r17 
.def r_LCD_TMPH			= r18 

.equ i_CMD_LCD_CLR		= 0x01
.equ i_CMD_LCD_DISPON	= 0x0C
.equ i_CMD_LCD_CURSON	= 0x0E
.equ i_CMD_LCD_BCURON	= 0x0F
.equ i_CMD_LCD_AUTOINC	= 0x06
.equ i_CMD_LCD_LINE1	= 0x80
.equ i_CMD_LCD_LINE2	= 0xC0	
.equ i_CMD_LCD_5x7_4BIT	= 0x28
.equ i_CMD_LCD_4BIT		= 0x20 ;0x02 ;0x20 ???

.equ i_LCD_CTRL_DDR		= DDRB
.equ i_LCD_CTRL_PORT	= PORTB
.equ i_LCD_RS_PIN		= PB2
.equ i_LCD_RW_PIN		= PB1
.equ i_LCD_E_PIN		= PB0

.equ i_LCD_DATA_DDR		= DDRD
.equ i_LCD_DATA_PORT	= PORTD
.equ i_LCD_DATA_PIN		= PIND
.equ i_LCD_DATA_7		= PD2
.equ i_LCD_DATA_6		= PD3
.equ i_LCD_DATA_5		= PD6
.equ i_LCD_DATA_4		= PD7
;========================================================
.macro m_LCD_WRCMD_MODE
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_RW_PIN ; RW = 0
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_RS_PIN ; RS = 0
.endmacro

.macro m_LCD_RDCMD_MODE
	m_SET_PIN_HIGH 	i_LCD_CTRL_PORT, i_LCD_RW_PIN ; RW = 1
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_RS_PIN ; RS = 0
.endmacro

.macro m_LCD_WRDATA_MODE
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_RW_PIN ; RW = 0
	m_SET_PIN_HIGH 	i_LCD_CTRL_PORT, i_LCD_RS_PIN ; RS = 1
.endmacro
;========================================================

;========================================================
; m_LCD_SEND_COMMAND i_CMD
.macro m_LCD_SEND_COMMAND
	m_LCD_WAIT_BUSY
	m_LCD_WRCMD_MODE
	push r_LCD_iTMP
	ldi r_LCD_iTMP, (@0) 
	rcall l_LCD_SEND_BYTE
	pop r_LCD_iTMP
	;m_PORT_WRITE PORTD, @0
.endmacro

; m_LCD_SEND_DATA i_CHAR
.macro m_LCD_SEND_DATA
	m_LCD_WAIT_BUSY
	m_LCD_WRDATA_MODE
	push r_LCD_iTMP
	ldi r_LCD_iTMP, (@0) 
	rcall l_LCD_SEND_BYTE
	pop r_LCD_iTMP
	;m_PORT_WRITE PORTD, @0
.endmacro

.macro m_LCD_WAIT_BUSY
	push r_LCD_iTMP
	rcall l_LCD_WAIT_BUSY
	pop r_LCD_iTMP
.endmacro

.macro m_LCD_LATCH_DATA
	rcall l_LCD_LATCH_DATA
.endmacro

l_LCD_LATCH_DATA:
	m_SET_PIN_HIGH 	i_LCD_CTRL_PORT, i_LCD_E_PIN 	; E = 1
	rcall l_LCD_DELAY
	m_SET_PIN_LOW	i_LCD_CTRL_PORT, i_LCD_E_PIN	; E = 0
	rcall l_LCD_DELAY
	ret
	
l_LCD_SEND_BYTE:
	rcall l_LCD_SEND_NIBBLE
	swap r_LCD_iTMP
	rcall l_LCD_SEND_NIBBLE
	ret

l_LCD_SEND_NIBBLE:
	cbi i_LCD_DATA_PORT, i_LCD_DATA_7
	cbi i_LCD_DATA_PORT, i_LCD_DATA_6
	cbi i_LCD_DATA_PORT, i_LCD_DATA_5
	cbi i_LCD_DATA_PORT, i_LCD_DATA_4
    sbrc r_LCD_iTMP, 7
    sbi i_LCD_DATA_PORT, i_LCD_DATA_7
	sbrc r_LCD_iTMP, 6
    sbi i_LCD_DATA_PORT, i_LCD_DATA_6
	sbrc r_LCD_iTMP, 5
    sbi i_LCD_DATA_PORT, i_LCD_DATA_5
	sbrc r_LCD_iTMP, 4
    sbi i_LCD_DATA_PORT, i_LCD_DATA_4
	m_LCD_LATCH_DATA
	ret

l_LCD_DELAY:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
ret
	
l_LCD_WAIT_BUSY:
	m_SET_PIN_LOW i_LCD_DATA_PORT, i_LCD_DATA_7
	m_SET_PIN_LOW i_LCD_DATA_PORT, i_LCD_DATA_6
	m_SET_PIN_LOW i_LCD_DATA_PORT, i_LCD_DATA_5
	m_SET_PIN_LOW i_LCD_DATA_PORT, i_LCD_DATA_4
	m_SET_PIN_INPUT i_LCD_DATA_DDR, i_LCD_DATA_7
	m_SET_PIN_INPUT i_LCD_DATA_DDR, i_LCD_DATA_6
	m_SET_PIN_INPUT i_LCD_DATA_DDR, i_LCD_DATA_5
	m_SET_PIN_INPUT i_LCD_DATA_DDR, i_LCD_DATA_4
	m_LCD_RDCMD_MODE
	m_SET_PIN_HIGH 	i_LCD_CTRL_PORT, i_LCD_E_PIN 	; E = 1
	rcall l_LCD_DELAY
	in r_LCD_iTMP, i_LCD_DATA_PIN
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_E_PIN 	; E = 0
	rcall l_LCD_DELAY
	m_SET_PIN_HIGH 	i_LCD_CTRL_PORT, i_LCD_E_PIN 	; E = 1
	rcall l_LCD_DELAY
	m_SET_PIN_LOW 	i_LCD_CTRL_PORT, i_LCD_E_PIN 	; E = 0
	sbrc r_LCD_iTMP, i_LCD_DATA_7
	rjmp l_LCD_WAIT_BUSY
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_7
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_6
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_5
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_4
ret
;========================================================

;========================================================
.macro m_INIT_LCD
	m_SET_PIN_OUTPUT i_LCD_CTRL_DDR, i_LCD_E_PIN
	m_SET_PIN_LOW	i_LCD_CTRL_PORT, i_LCD_E_PIN	; E = 0
	m_SET_PIN_OUTPUT i_LCD_CTRL_DDR, i_LCD_RS_PIN
	m_SET_PIN_OUTPUT i_LCD_CTRL_DDR, i_LCD_RW_PIN
	m_LCD_WRDATA_MODE
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_7
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_6
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_5
	m_SET_PIN_OUTPUT i_LCD_DATA_DDR, i_LCD_DATA_4
	m_LCD_SEND_COMMAND i_CMD_LCD_4BIT
	m_LCD_SEND_COMMAND i_CMD_LCD_4BIT
	m_LCD_SEND_COMMAND i_CMD_LCD_5x7_4BIT
	m_LCD_SEND_COMMAND i_CMD_LCD_5x7_4BIT ;display hw bug ?? needs to be sent twice
	m_LCD_SEND_COMMAND i_CMD_LCD_AUTOINC
	m_LCD_SEND_COMMAND i_CMD_LCD_DISPON
	m_LCD_SEND_COMMAND i_CMD_LCD_CLR
	m_LCD_SEND_COMMAND i_CMD_LCD_LINE1
.endmacro
;========================================================

;========================================================
.macro m_PRINT_STRING
	push ZL
	push ZH
	push r_LCD_iTMP
	ldi ZL, LOW(@0)
    ldi ZH, HIGH(@0)
	rcall l_PRINT_STRING
	pop r_LCD_iTMP
	pop ZH
	pop ZL
.endmacro

l_PRINT_STRING:
	lsl     ZL
	rol     ZH
l_PRINT_STRING_RPT:
    lpm r_LCD_iTMP, Z+
    cpi r_LCD_iTMP, 0x00
    breq l_PRINT_STRING_EXIT 
	m_LCD_WAIT_BUSY
	m_LCD_WRDATA_MODE
	rcall l_LCD_SEND_BYTE
    rjmp l_PRINT_STRING_RPT
	l_PRINT_STRING_EXIT:
	ret
;========================================================

