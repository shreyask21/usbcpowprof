; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Macros for GPIO.
; ----------------------------------------------

; scrachpad reg for gpio operations
.def r_GPIO = r16 

;========================================================
; m_SET_PIN_INPUT (DDRx,PINNUM)
.macro m_SET_PIN_INPUT
	cbi (@0), (@1)
.endmacro

; TODO FIX
; m_READ_PIN (PINx,PINNUM,Rd)
.macro m_READ_PIN
	push r_GPIO
	in r_GPIO, (@0)
	bst r_GPIO, (@1)
	bld r_GPIO, 0
	andi r_GPIO, 0x01
	mov (r@2),r_GPIO
	pop r_GPIO
.endmacro
;========================================================

;========================================================
; m_SET_PIN_OUTPUT (DDRx,PINNUM)
.macro m_SET_PIN_OUTPUT
	sbi (@0), (@1)
.endmacro

; m_SET_PIN_HIGH (PORTx,PINNUM)
.macro m_SET_PIN_HIGH
	sbi (@0), (@1)
.endmacro

; m_SET_PIN_LOW (PORTx,PINNUM)
.macro m_SET_PIN_LOW
	cbi (@0), (@1)
.endmacro
;========================================================

;========================================================
; m_SET_PORT_INPUT (DDRx)
.macro m_SET_PORT_INPUT
	push r_GPIO
	clr r_GPIO
	out (@0), r_GPIO
	pop r_GPIO
.endmacro

; m_PORT_READ (PINx, Rd)
.macro m_PORT_READ
	in (@0),(r@1)
.endmacro
;========================================================

;========================================================
; m_SET_PORT_OUTPUT (DDRx)
.macro m_SET_PORT_OUTPUT
	push r_GPIO
	ldi r_GPIO, 0xFF
	out (@0), r_GPIO
	pop r_GPIO
.endmacro

; m_PORT_WRITE (PORTx, DATA)
.macro m_PORT_WRITE
	push r_GPIO
	ldi r_GPIO, (@1)
	out (@0), r_GPIO
	pop r_GPIO
.endmacro
;========================================================
