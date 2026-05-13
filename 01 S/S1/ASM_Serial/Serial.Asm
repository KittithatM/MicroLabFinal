T2CON		EQU	0C8H
RCAP2L		EQU	0CAH 
RCAP2H		EQU	0CBH 

DBufffer_4Byte	EQU	30H	; Buffer 4 Byte
CounterH	EQU	34H	 
CounterL	EQU	35H	
	 
PortO_LEDGreen	EQU	P3.7	
	
		ORG	0000H	
		JMP	0100H
	
		ORG	0100H
		MOV	SP,#3FH
		CLR	EA	
		CALL	Delay_250M	
		CALL	Initial_Serial	
		CALL	Send_ClrScr
		
Main_Loop:		
		MOV	DPTR,#Header_Fram	
		CALL	Send_Table	
			
		MOV	CounterH,TH0
		MOV	CounterL,TL0	
		MOV	A,CounterH	
		CALL	Send_2HEX
		MOV	A,CounterL	
		CALL	Send_2HEX
		MOV	A,#','
		CALL	Send_ASCII
		MOV	DPH,CounterH
		MOV	DPL,CounterL
		CALL	DPTR2BCD	; DPTR(H) --> B-DPH-DPL(BCD)
		MOV	A,B	
		CALL	Send_2HEX
		MOV	A,DPH	
		CALL	Send_2HEX
		MOV	A,DPL	
		CALL	Send_2HEX
		
		MOV	DPTR,#D2323
		CALL	Send_Table
		
		MOV	CounterH,TH1
		MOV	CounterL,TL1	
		MOV	A,CounterH	
		CALL	Send_2HEX
		MOV	A,CounterL	
		CALL	Send_2HEX
		MOV	A,#','
		CALL	Send_ASCII
		MOV	DPH,CounterH
		MOV	DPL,CounterL
		CALL	DPTR2BCD	; DPTR(H) --> B-DPH-DPL(BCD)
		MOV	A,B	
		CALL	Send_2HEX
		MOV	A,DPH	
		CALL	Send_2HEX
		MOV	A,DPL	
		CALL	Send_2HEX
		
		MOV	DPTR,#EDDD	
		CALL	Send_Table
			
		CALL	Send_LineFeed	
		CPL	PortO_LEDGreen	
		
		CALL	Delay_500M	
		
		JMP	Main_Loop	

$INCLUDE (myDelay.A51)
$INCLUDE (mySerial.A51)
$INCLUDE (myHex2BCD.A51)

Header_Fram:	DB	'D22 [',00
D2323:		DB	'] | D23 [',00
EDDD:		DB	']',00
		
		
		END
