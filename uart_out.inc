; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    		Macros for UART
; ----------------------------------------------

.equ    F_CPU			= 16000000
.equ	baud_rate		= 100000
.equ	baud_rate_psc	= (((F_CPU / (baud_rate * 16))) - 1)

.macro m_UART_INIT
	rcall l_UART_INIT
.endmacro

.macro m_UART_TRANSMIT_CHAR
	rcall l_UART_TRANSMIT_CHAR
.endmacro

.macro m_UART_TRANSMIT_COMMA
	rcall l_UART_TRANSMIT_COMMA
.endmacro

.macro m_UART_TRANSMIT_CRLF
	rcall l_UART_TRANSMIT_CRLF
.endmacro

l_UART_INIT:
	push r16
	push r17
	ldi	r16,LOW(baud_rate_psc)
	ldi	r17,HIGH(baud_rate_psc)
	sts	UBRR0L,r16
	sts	UBRR0H,r17
	ldi	r16,(1<<RXEN0)|(1<<TXEN0)
	sts	UCSR0B,r16
	; Set frame format: 8N1
	ldi r16,  (1 << UCSZ01) | (1 << UCSZ00)
	sts UCSR0C, r16
	pop r17
	pop r16
	ret

l_UART_TRANSMIT_CHAR:
	push	r17	
	l_UART_TRANSMIT_CHAR_REP:
		lds		r17,UCSR0A
		sbrs	r17,UDRE0
		rjmp	l_UART_TRANSMIT_CHAR_REP
	sts		UDR0,	r16
	pop		r17
	ret		
	
l_UART_TRANSMIT_COMMA:
	push	r16	
	ldi		r16, ','
	rcall l_UART_TRANSMIT_CHAR
	pop		r16
	ret			

l_UART_TRANSMIT_CRLF:
	push	r16	
	ldi		r16, 0x0D  ;CR
	rcall l_UART_TRANSMIT_CHAR
	ldi		r16, 0x0A  ;LF
	rcall l_UART_TRANSMIT_CHAR
	pop		r16
	ret					