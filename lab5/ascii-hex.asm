		AREA RESET,DATA,READONLY
		EXPORT __Vectors
	
__Vectors
		DCD 0x40001000
		DCD Reset_Handler
		
		AREA ascii2hex,CODE,READONLY
		EXPORT Reset_Handler
		ENTRY
	
Reset_Handler
		MOV R4,#8
		MOV R3,#0
		LDR R1,=array
UP		LSL R3,#4
		LDRB R2,[R1],#1
		CMP R2,#0x41
		SUBLO R2,#0x30
		SUBHS R2,#0x37
		ORR R3,R2
		SUB R4,#1
		TEQ R4,#0
		BNE UP
STOP	B STOP

array DCB "12AB34CD"
		END