		AREA RESET,DATA,READONLY
		EXPORT __Vectors
	
__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA hex2ascii,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY
	
Reset_Handler
		MOV R5,#8
		LDR R1,=num
		LDR R2,[R1]
		LDR R3,=array
		ADD R3,#7
UP		AND R4,R2,0x0F
		CMP R4,#0x0A
		ADDLO R4,#0x30
		ADDHS R4,#0x37
		STRB R4,[R3],#-1
		LSR R2,#4
		SUB R5,#1
		TEQ R5,#0
		BNE UP
STOP	B STOP

num dcd 0x12AB34CD