		AREA RESET,DATA,READONLY
		EXPORT __Vectors
		
__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA mycode,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY
		
Reset_Handler
		LDR R0,=hexval
		LDR R1,[R0]
		MOV R7,#32
		MOV R3,#0x0A
		MOV R6,#0
		MOV R2,R1
UP		MOV R4,#0
		BL divide
		ORR R6,R2
		ROR R6,#4
		SUB R7,#4
		MOV R2,R4
		CMP R4,#0
		BNE UP
		LSR R6,R7
		LDR R0,=bcdval
		STR R6,[R0]
STOP	B STOP

divide	CMP R2,R3
		SUBHS R2,R3
		ADDHS R4,#1
		BHS divide
		BX LR
		
hexval	DCD 0x3A

		AREA mydata,DATA,READWRITE
bcdval	DCD 0
		
		END