; ----------------------------------------------
;    --------- USBC Power Profiler ----------
;		github.com/shreyask21/usbcpowprof 
;    			Strings
; ----------------------------------------------

s_SPLASH_STR_1:
	.db "      USB C     ",0x00,0x00

s_SPLASH_STR_2:
	.db " Power Profiler ",0x00,0x00

s_SPLASH_STR_4:
	.db "      GitHub    ",0x00,0x00

s_SPLASH_STR_3:
	.db "   @shreyask21  ",0x00

s_mV:
	.db "mV ",0x00,0x00
s_mA:
	.db "mA",0x00,0x00
	
s_FOOTER_BW:
	.db "USBC1 ",127,"--- USBC2",0x00,0x00
	
s_FOOTER_FW:
	.db "USBC1 ---",126," USBC2",0x00,0x00
	
s_SPACE:
	.db " ",0x00

s_BLANK_STRING:
	.db "                ",0x00,0x00
