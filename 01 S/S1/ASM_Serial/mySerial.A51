
;============  Initial Serial Port   ============
Initial_Serial:	MOV	TMOD,#55H	; T1M2 T0M1 0010 0101
		MOV	SCON,#50H	; Serial Mode1£
		MOV	A,#00H 	; FBH@18.432MHz
		MOV	PCON,A	; SMOD = 0
		
		MOV	T2CON,#34H
		MOV	RCAP2H,#0FFH
		MOV	RCAP2L,#0DCH
		
		CLR	EA	; Disable All 
		SETB	TR0	; Start Timer1
		SETB	TR1
		RET		
			
;==========  Send Data to Serial Port   ==========
SEND_2HEX:	PUSH	Acc	
		SWAP	A	
		CALL	SEND_1HEX	
		POP	Acc	
SEND_1HEX:	PUSH	DPH	
		PUSH	DPL	
		MOV	DPTR,#HEXASC_TAB	
		ANL	A,#0FH	
		MOVC	A,@A+DPTR	
		POP	DPL	
		POP	DPH	
SEND_ASCII:	PUSH	IE	
		CLR	ES	
		CLR    	TI	
		MOV    	SBUF,A	
		JNB    	TI,$	
		CLR    	TI	
		POP	IE	
		RET		
HEXASC_TAB: 	DB	'0123456789ABCDEF'	
			
;=========   Send Line Feed =================
Send_LineFeed:	MOV	A,#0DH	
		CALL	Send_ASCII	
		MOV	A,#0AH	
		CALL	Send_ASCII	
		RET		
			
;========  Send Line Feed ==================
Send_ClrScr:	MOV	A,#0CH	
		CALL	Send_ASCII	
		RET		
			
;== Send Data Table to Serial Start@DPTR to '00H' =
Send_Table:	CLR	A	
		MOVC	A,@A+DPTR	
		JZ	_X_Send_Tab	
		CALL	SEND_ASCII	
		INC	DPTR	
		JMP	Send_Table	
_X_Send_Tab:	RET		
			
;========= Send 1-Byte@Acc to SERIAL =========
TX_BYTE:	PUSH	IE	
		CLR	ES	
		CLR	TI	
		MOV	SBUF,A	
		JNB	TI,$	
		CLR	TI	
		POP	IE	
		RET		
			
;====== Receive Data From SERIAL to Acc ========
RX_BYTE:	PUSH	IE	
		CLR	ES	
		JNB	RI,$	; Wait data
		CLR	RI	
		MOV	A,SBUF	
		POP	IE	
		RET 		
