		AREA RESET,CODE,READONLY
		EXPORT __Vectors
		
__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA mycode,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY
		
Reset_Handler
		LDR R0,=m
		LDR R1,=n
		LDR R2,=prod
		LDR R3,[R0]	
		LDR R4,[R1]	;counter
		MOV R5,#0	;sum clear
		MOV R6,#0	;carry clear
BACK	ADDS R5,R3	;add itself
		ADC R6,#0	;add carry
		SUB R4,#1	;decrement
		CMP R4,#0
		BNE BACK
		STR R5,[R2],#4
		STR R6,[R2]
		
STOP	B STOP
m 		DCD 98
n		DCD 56
		
		AREA mydata,DATA,READWRITE
prod	DCD 0,0
		END