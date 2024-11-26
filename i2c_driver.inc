; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Macros for I2C/TWI.
; ----------------------------------------------

; scrachpad reg for TWI operations
.def r_TWI_DH = r17
.def r_TWI_DL = r16 
.def r_TWI_TMP = r18

.equ i_I2C_SCL_400KHZ = 0x12

.macro m_INIT_I2C
	rcall l_INIT_I2C
.endmacro

.macro m_I2C_WRITE8
	push r_TWI_TMP
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0)		;	device addr
	lsl r_TWI_TMP
	cbr r_TWI_TMP,0			;	SLA+W
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, (@1)		;	reg addr
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, (@2)		;	reg data
	rcall l_I2C_WRITE8
	rcall l_I2C_STOP
	pop r_TWI_TMP
.endmacro

.macro m_I2C_WRITE16
	push r_TWI_TMP
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0)		;	device addr
	lsl r_TWI_TMP
	cbr r_TWI_TMP,0			;	SLA+W
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, (@1)		;	reg addr	
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, HIGH(@2)	;	reg data,high byte	
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, LOW(@2)	;	reg data,low byte
	rcall l_I2C_WRITE8
	rcall l_I2C_STOP
	pop r_TWI_TMP
.endmacro

.macro m_I2C_READ8
	push r_TWI_TMP
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0) 	;	device addr
	lsl r_TWI_TMP
	cbr r_TWI_TMP,0			;	SLA+W
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, (@1)		;	reg addr
	rcall l_I2C_WRITE8
	rcall l_I2C_STOP
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0) 	;	device addr
	lsl r_TWI_TMP
	sbr r_TWI_TMP,1			;	SLA+R
	rcall l_I2C_WRITE8
	rcall l_I2C_ACK
	lds r_TWI_DL, TWDR
	;rcall l_I2C_STOP
	pop r_TWI_TMP
.endmacro

.macro m_I2C_READ16
	push r_TWI_TMP
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0) 	;	device addr
	lsl r_TWI_TMP
	cbr r_TWI_TMP,1			;	SLA+W
	rcall l_I2C_WRITE8
	ldi r_TWI_TMP, (@1)		;	reg addr
	rcall l_I2C_WRITE8
	rcall l_I2C_STOP
	
	rcall l_I2C_START
	ldi r_TWI_TMP, (@0) 	;	device addr
	lsl r_TWI_TMP
	sbr r_TWI_TMP,1			;	SLA+R
	rcall l_I2C_WRITE8
	
	rcall l_I2C_ACK
	lds r_TWI_DH, TWDR
	rcall l_I2C_ACK
	lds r_TWI_DL, TWDR
	rcall l_I2C_NACK
	rcall l_I2C_STOP
	pop r_TWI_TMP
.endmacro

;========================================================
l_INIT_I2C:
	push r_TWI_TMP
	; enable pullups
	m_SET_PIN_INPUT DDRC, PC4
	m_SET_PIN_INPUT DDRC, PC5
	m_SET_PIN_HIGH PORTC, PC4
	m_SET_PIN_HIGH PORTC, PC5
	
	; prescaler
	ldi r_TWI_TMP, 0x00
	sts TWSR, r_TWI_TMP
	
	; bitrate
	ldi r_TWI_TMP, i_I2C_SCL_400KHZ
	sts TWBR, r_TWI_TMP
	
	; data buf clear
	clr r_TWI_TMP
	sts TWDR, r_TWI_TMP
	
	; enable periph
	ldi r_TWI_TMP, (1<<TWEN)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
ret

l_I2C_START:
	push r_TWI_TMP
	ldi r_TWI_TMP, (1<<TWEN)|(1<<TWINT)|(1<<TWSTA)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
	rcall l_I2C_PERIPH_WAIT
ret

l_I2C_STOP_WAIT:
	push r_TWI_TMP
l_I2C_STOP_WAIT_2:
	lds r_TWI_TMP, TWCR
	andi r_TWI_TMP, (1<<TWSTO)
	cpi r_TWI_TMP, (1<<TWSTO)
	breq l_I2C_STOP_WAIT_2
	pop r_TWI_TMP
ret

l_I2C_STOP:
	push r_TWI_TMP
	ldi r_TWI_TMP, (1<<TWEN)|(1<<TWINT)|(1<<TWSTO)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
	rcall l_I2C_STOP_WAIT
ret

l_I2C_WRITE8:
	sts TWDR, r_TWI_TMP
	push r_TWI_TMP
	ldi r_TWI_TMP, (1<<TWEN)|(1<<TWINT)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
	rcall l_I2C_PERIPH_WAIT
ret

l_I2C_ACK:
	push r_TWI_TMP
	clr r_TWI_TMP
	sts TWDR, r_TWI_TMP
	ldi r_TWI_TMP, (1<<TWEN)|(1<<TWINT)|(1<<TWEA)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
	rcall l_I2C_PERIPH_WAIT
ret

l_I2C_NACK:
	push r_TWI_TMP
	clr r_TWI_TMP
	sts TWDR, r_TWI_TMP
	ldi r_TWI_TMP, (1<<TWEN)|(1<<TWINT)
	sts TWCR, r_TWI_TMP
	pop r_TWI_TMP
	rcall l_I2C_PERIPH_WAIT
ret

l_I2C_PERIPH_WAIT:
	push r_TWI_TMP
l_I2C_PERIPH_WAIT_2:
	lds r_TWI_TMP, TWCR
	sbrs r_TWI_TMP, TWINT
	rjmp l_I2C_PERIPH_WAIT_2
	pop r_TWI_TMP
ret
;========================================================
