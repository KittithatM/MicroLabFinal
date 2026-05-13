

DPTR2BCD:	MOV	R7,#0
		MOV	R6,#0
		MOV	R5,DPH
		MOV	R4,DPL
		MOV	R0,#DBufffer_4Byte
		; Input  : R7,R6,R5,R4(HEX) 
		; Output : @R0+0 +1 +2 +3(BCD)
		CALL	HEX2BCD_4Byte
		MOV	R0,#DBufffer_4Byte+1
		MOV	A,@R0
		MOV	B,A
		INC	R0
		MOV	A,@R0
		MOV	DPH,A
		INC	R0
		MOV	A,@R0
		MOV	DPL,A
		RET
		
;==============================================================
;             Four Byte x One Byte
;      Input  :    R6,R5,R4,R3 x R0 
;      Output : R7,R6,R5,R4,R3 
;==============================================================
MUL_4Byte_1Byte:	NOP
;-------------------------------------	
_M_4B1B_Byte1:	MOV	A,R0
		MOV	B,R3
		MUL	AB
		MOV	R3,A	; Byte1
		MOV	R1,B
;-------------------------------------	
_M_4B1B_Byte2:	MOV	A,R0
		MOV	B,R4
		MUL	AB
		CLR	C
		ADD	A,R1
		MOV	R4,A	; Byte2
		MOV	A,#00
		ADDC	A,B
		MOV	R1,A
;-------------------------------------	
_M_4B1B_Byte3:	MOV	A,R0
		MOV	B,R5
		MUL	AB
		CLR	C
		ADD	A,R1
		MOV	R5,A	; Byte3
		MOV	A,#00
		ADDC	A,B
		MOV	R1,A
;-------------------------------------	
_M_4B1B_Byte4:	MOV	A,R0
		MOV	B,R6
		MUL	AB
		CLR	C
		ADD	A,R1
		MOV	R6,A	; Byte4
		MOV	A,#00
		ADDC	A,B
		MOV	R7,A	; Byte5
		RET
	
;==============================================================
;             Four Byte Hex to BCD Converstor 
;      Input  : R7,R6,R5,R4(HEX) 
;      Output : @R0+0 +1 +2 +3(BCD)
;==============================================================
HEX2BCD_4Byte: 	CLR	A
            	MOV 	@R0,A	; MSB
            	INC	R0
            	MOV  	@R0,A	 
            	INC	R0
            	MOV 	@R0,A	 
            	INC	R0
            	MOV  	@R0,A	; LSB
            	MOV	R1,00H	; Back Up R0
	
            	MOV  	DPH,#32D        ; Data 32 Bits(4Byte)
_HEXTOBCD01_4B: MOV   	A,R4
            	RLC   	A
            	MOV   	R4,A
            	MOV  	A,R5
            	RLC  	A
            	MOV    	R5,A
            	MOV   	A,R6
            	RLC    	A
            	MOV    	R6,A
            	MOV   	A,R7
            	RLC   	A
            	MOV    	R7,A
            	
            	MOV   	DPL,#04D	; 3Byte Output
            	MOV   	R0,01H		; Index To Address@R0
_HEXTOBCD02_4B: MOV     A,@R0
            	ADDC 	A,Acc
            	DA   	A
            	MOV   	@R0,A
            	DEC	R0
            	DJNZ  	DPL,_HEXTOBCD02_4B
            	DJNZ   	DPH,_HEXTOBCD01_4B
            	RET	


;#######################################################################
;======================================================================
;  Converse DPTR(HEX) to BCD ==> Hex lessthan 270FH(9999)
;======================================================================
DPTR_HEX2BCD:	MOV	R4,DPH	; Load Input
		MOV	R3,DPL
		MOV	R6,#0	; Clear H_Output
		MOV	R7,#0	; Clear L_Output
            	MOV 	DPH,#16D        	; Data 2Byte(16 Bits)
_DPTR_HEX2BCD01:MOV  	A,R3
            	RLC  	A
            	MOV  	R3,A
            	MOV  	A,R4
            	RLC   	A
            	MOV  	R4,A
            	MOV  	DPL,#02D        	; 2Byte Output
            	MOV    	R0,#07H	; Index To Address@R0
_DPTR_HEX2BCD02: MOV 	A,@R0
            	ADDC  	A,Acc
            	DA    	A
            	MOV   	@R0,A
            	DEC	R0
            	DJNZ  	DPL,_DPTR_HEX2BCD02
            	DJNZ  	DPH,_DPTR_HEX2BCD01
            	MOV	DPH,R6
            	MOV	DPL,R7
            	RET
