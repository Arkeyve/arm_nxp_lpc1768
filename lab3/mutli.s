		AREA RESET,DATA,READONLY
		EXPORT __Vectors

__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA multi,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY

Reset_Handler
		LDR R0,=num1
		LDR R1,=num2
		LDR R2,[R0]
		LDR R3,[R1]
		MOV R4,#0
UP		MOV R6,#0
		ADDS R4,R2
		ADDCS R5,#1
		SUB R3,#1
		TEQ R3,#0
		BNE UP
		LDR R0,=res
		STR R4,[R0],#4
		STR R5,[R0]
STOP	B STOP

num1	DCD 0x1A1873EF
num2	DCD 0x0F
	
		AREA result,DATA,READWRITE
res		DCD 0,0
	
		END