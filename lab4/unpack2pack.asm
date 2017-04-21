	AREA RESET,DATA,READONLY
	EXPORT __Vectors
	
__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	
	AREA u2p,CODE,READONLY
	EXPORT Reset_Handler
	ENTRY
	
Reset_Handler
	LDR R0,=unpacked
	LDR R1,=packed
	MOV R4,#8
UP	LDRB R3,[R0],#1
	ORR R1,R3
	SUB R4,#1
	CMP R4,#0
	LSLNE R1,#4
	BNE UP
	
STOP
	B STOP
	
unpacked
	DCB 1,2,3,4,5,6,7,8

	AREA packloc,DATA,READWRITE
packed
	DCD 0
	
	END