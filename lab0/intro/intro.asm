	AREA RESET,DATA,READONLY
	EXPORT __Vectors

__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	
	AREA mycode,CODE,READONLY
	EXPORT Reset_Handler
	ENTRY

Reset_Handler
	LDR R0,=0xA
	LDR R1,=0xB
	
STOP
	B STOP
	END