
;############################################
;#************************************************
;#      All Delay for XTAL 18.432MHz 12Clk/MC	
;#************************************************
DELAY_4SEC:	PUSH	07H	; Delay 80*50MilliSec
		MOV	R7,#80	;  = 4000MilliSec
		JMP	_DELAY_50M	
			
DELAY_2SEC:	PUSH	07H	; Delay 40*50MilliSec
		MOV	R7,#40	;  = 2000MilliSec
		JMP	_DELAY_50M	
			
DELAY_1SEC:	PUSH	07H	; Delay 20*50MilliSec
		MOV	R7,#20	;  = 1000MilliSec
		JMP	_DELAY_50M	
			
DELAY_500M:	PUSH	07H	; Delay 10*50MilliSec
		MOV	R7,#10	;  = 500MilliSec
		JMP	_DELAY_50M	
			
DELAY_250M:	PUSH	07H	; Delay 5*50 MilliSec
		MOV	R7,#5	;  = 250 MilliSec
		JMP	_DELAY_50M	
			
DELAY_100M:	PUSH	07H	; Delay 2*50 MilliSec
		MOV	R7,#2	;  = 100 MilliSec
		JMP	_DELAY_50M	
			
DELAY_50M:	PUSH	07H	; Delay 1*50 MilliSec
		MOV	R7,#1	;  = 50 MilliSec
		JMP	_DELAY_50M	
			
 
_DELAY_50M:	PUSH	Acc	
		PUSH	B	
_DELAY_050M:	MOV	B,#200	; Delay 50 MilliSec
_DELAY_250U:	MOV	A,#114	; 1MC ,250 uSec
		DJNZ	Acc,$	; 2MC ,(192-1)*0.651*2 = 250 uSec
		DJNZ	B,_DELAY_250U 	; 250*200=50mS
		DJNZ	R7,_DELAY_050M	; 50*5 =250mS
		POP	B	
		POP	Acc	
		POP	07H	
		RET		
