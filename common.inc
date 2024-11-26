; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		 Common Macros
; ----------------------------------------------

.equ i_DELAY_VAL = 39998

.macro m_INIT_STACK
	ldi	r16,LOW(RAMEND)
	out	SPL,r16
	ldi	r16,HIGH(RAMEND)
	out	SPH,r16
.endmacro

.macro m_POWERSAVE
	push r16
	rcall l_POWERSAVE
	pop r16
.endmacro

.macro m_DELAY_MS
	push r16
	push r24
	push r25
	ldi r16, (@0/10)
	rcall l_delay
	pop r25
	pop r24
	pop r16
.endmacro

l_delay:
	ldi	r24,LOW(i_DELAY_VAL)
	ldi	r25,HIGH(i_DELAY_VAL)
l_delay_loop:	
	sbiw r24,1
	brne l_delay_loop
	dec r16
	brne l_delay
	nop
	ret

l_POWERSAVE:
	; Disable ADC, SPI, and Timers to save power
	ldi r16, 0x2D
	sts PRR, r16
	; Disable Timer0 and Timer1, Timer2
	ldi r16, 0
	sts TCCR0B, r16   ; Stop Timer0
	sts TCCR1B, r16   ; Stop Timer1
	sts TCCR2B, r16   ; Stop Timer2
	; Disable ADC to save power
	ldi r16, (0<<ADEN)
	sts ADCSRA, r16
	; Disable Analog Comparator
	ldi r16, (0<<ACD)
	sts ACSR, r16
	; Configure all pins as inputs and drive them low to reduce power consumption
	ldi r16, 0x00
	out DDRB, r16		; Set all PORTB pins as inputs
	out DDRC, r16		; Set all PORTC pins as inputs
	out DDRD, r16		; Set all PORTD pins as inputs
	out PORTB, r16		; Disable pullups
	out PORTC, r16		; Disable pullups
	out PORTD, r16		; Disable pullups
ret