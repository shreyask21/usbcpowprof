; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    Macros for playing tones over buzzer
; ----------------------------------------------


.macro m_BUZZER
	push r16
	ldi r16, (255 - (32*(@0)))
	rcall l_PLAY_TONE
	m_DELAY_MS (@1)
	rcall l_DISABLE_TONE
	pop r16
.endmacro


l_PLAY_TONE:
	; load tone freq
	sts OCR2A, r16
	; Set Timer2 to CTC mode, toggle OC2 on compare match
	ldi r16, (1 << WGM21) | (1 << COM2A0) 
    sts TCCR2A, r16
    ldi r16, (1 << CS22)	; Prescaler 64
    sts TCCR2B, r16	; Set PB3 as output for OC2A
	sbi DDRB, PB3
ret

l_DISABLE_TONE:
	ldi r16, 0x00
    sts TCCR2A, r16
    sts TCCR2B, r16
	cbi DDRB, PB3
ret