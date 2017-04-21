		AREA RESET,CODE,READONLY
		EXPORT __Vectors
		
__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA mycode,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY
		
Reset_Handler
		LDR R0,=n
		LDR R1,=sum
		LDR R2,[R0]	;counter
		MOV R3,#0	;sum clear
		MOV R4,#0	;carry clear
BACK	ADDS R3,R2
		ADC R4,#0	;add carry
		SUB R2,#1	;decrement
		CMP R2,#0
		BNE BACK
		STR R3,[R1],#4
		STR R4,[R1]
		
STOP	B STOP
n 		DCD 100
		
		AREA mydata,DATA,READWRITE
sum		DCD 0,0
		END