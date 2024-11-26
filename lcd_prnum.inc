; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;     Macros for Displaying Numbers on LCD.
; ----------------------------------------------

.def	r_BCD_IN_L	=r16		;binary value Low byte =r16
.def	r_BCD_IN_H	=r17		;binary value High byte =r17

.macro m_LCD_PRINTu16
	push r_BCD_IN_L
	push r_BCD_IN_H
	ldi r_BCD_IN_L, LOW(@0)
	ldi r_BCD_IN_H, HIGH(@0) 
	rcall l_LCD_PRINTu16
	pop r_BCD_IN_H
	pop r_BCD_IN_L
.endmacro

.macro m_LCD_PRINTs16
	push r_BCD_IN_L
	push r_BCD_IN_H
	ldi r_BCD_IN_L, LOW(@0)
	ldi r_BCD_IN_H, HIGH(@0)
	rcall l_LCD_PRINTs16
	pop r_BCD_IN_H
	pop r_BCD_IN_L
.endmacro

.macro m_LCD_PRINTu8
	push r_BCD_IN_L
	ldi r_BCD_IN_L, (@0)
	rcall l_LCD_PRINTu8
	pop r_BCD_IN_L
.endmacro

.macro m_LCD_PRINTs8
	push r_BCD_IN_L
	ldi r_BCD_IN_L, (@0)
	rcall l_LCD_PRINTs8
	pop r_BCD_IN_L
.endmacro

l_LCD_SEND_BCD_NIB:
	andi r_LCD_iTMP, 0x0F 
	ori r_LCD_iTMP, '0'
	m_LCD_WAIT_BUSY
	m_LCD_WRDATA_MODE
	rcall l_LCD_SEND_BYTE
	ret

l_LCD_PRINTs16:
	tst r17
	brpl  l_16PLUS_SIGN
	m_LCD_SEND_DATA '-'
	com     r16             ; Complement the low byte (r16)
    com     r17             ; Complement the high byte (r17)
    subi    r16, 0xFF       ; Add 1 to the low byte (r16), with carry
    sbci    r17, 0xFF       ; Add carry to the high byte (r17)
	rjmp l_s16_PRINT
	l_16PLUS_SIGN:
		m_LCD_SEND_DATA '+'
	l_s16_PRINT:
		rcall l_LCD_PRINTu16
		ret

l_LCD_PRINTu16:
	push r13
	push r14
	push r15
	push r18
	push r19
	push ZL
	push ZH

	ldi	r18,16	;Init loop counter	
	clr	r15		;clear result (3 bytes)
	clr	r14		
	clr	r13		
	clr	ZH
	l_CONV_BCD16:
		lsl	r_BCD_IN_L		;shift input value
		rol	r_BCD_IN_H		;through all bytes
		rol	r13		;
		rol	r14
		rol	r15
		dec	r18		;decrement loop counter
		brne	l_CONV_BCD16_2		;if counter not zero

		mov r_LCD_iTMP, r15 ; bcd digit 4
		rcall l_LCD_SEND_BCD_NIB

		swap r14 ; bcd digit 3 (MSB->LSB)
		mov r_LCD_iTMP, r14 
		rcall l_LCD_SEND_BCD_NIB

		swap r14 ; bcd digit 2
		mov r_LCD_iTMP, r14 
		rcall l_LCD_SEND_BCD_NIB
	
		swap r13 ; bcd digit 3 (MSB->LSB)
		mov r_LCD_iTMP, r13 
		rcall l_LCD_SEND_BCD_NIB

		swap r13 ; bcd digit 2
		mov r_LCD_iTMP, r13 
		rcall l_LCD_SEND_BCD_NIB

		pop ZH
		pop ZL
		pop r19
		pop r18
		pop r15
		pop r14
		pop r13
	ret			;   return

l_CONV_BCD16_2:
	ldi	r30,15+1	;Z points to result MSB + 1
	l_CONV_BCD16_3:
		ld		r19,-Z	;get (Z) with pre-decrement
		subi	r19,-$03	;add 0x03
		sbrc	r19,3	;if bit 3 not clear
		st		Z,r19	;	store back
		ld		r19,Z	;get (Z)
		subi	r19,-$30	;add 0x30
		sbrc	r19,7	;if bit 7 not clear
		st		Z,r19	;	store back
		cpi		ZL,13	;done all three?
		brne	l_CONV_BCD16_3		;loop again if not
		rjmp	l_CONV_BCD16		;fml

l_LCD_PRINTs8:
	tst r_BCD_IN_L
	brpl  l_8PLUS_SIGN
	m_LCD_SEND_DATA '-'
	subi r_BCD_IN_L, 0x01
	com r_BCD_IN_L
	rjmp l_s8_PRINT
	l_8PLUS_SIGN:
		m_LCD_SEND_DATA '+'
	l_s8_PRINT:
		rcall l_LCD_PRINTu8
		ret

l_LCD_PRINTu8:
	push r_BCD_IN_H
	clr	r_BCD_IN_H
	rcall l_CONV_BCD8
	push r_BCD_IN_L
	mov r_BCD_IN_L, r_BCD_IN_H
	clr r_BCD_IN_H
	rcall l_CONV_BCD8
	rcall l_LCD_SEND_BCD_NIB
	mov r_BCD_IN_H, r_BCD_IN_L
	rcall l_LCD_SEND_BCD_NIB
	pop r_BCD_IN_L
	mov r_BCD_IN_H, r_BCD_IN_L
	rcall l_LCD_SEND_BCD_NIB
	pop r_BCD_IN_H
	ret
	
l_CONV_BCD8:
	subi	r_BCD_IN_H,10			;	input = input - 10
	brcs	l_CONV_BCD8_2			;	abort if carry set
	inc	    r_BCD_IN_L				;	inc MSD
	rjmp	l_CONV_BCD8				;	loop again
	l_CONV_BCD8_2:
		subi	r_BCD_IN_H,-10		;	compensate extra subtraction
ret