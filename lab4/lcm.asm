	AREA RESET,DATA,READONLY
	EXPORT __Vectors

__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	
	AREA lcm,CODE,READONLY
	EXPORT Reset_Handler
	ENTRY
	
Reset_Handler
	LDR R0,=num1
	LDR R1,=num2
	MOV R3,#1
	LDR R4,=lcmresult
	LDR R5,[R0]
	LDR R6,[R1]
lcmloop
	MUL R7,R5,R3
	MOV R8,R7
div	CMP R8,R6
	SUBHS R8,R8,R6
	BHS div
	CMP R8,#0
	ADDNE R3,#1
	BNE lcmloop
	STR R7,[R4]
	
STOP
	B STOP
	
num1 DCD 0x9
num2 DCD 0x6

	AREA lcmres,DATA,READWRITE
lcmresult
	DCD 0
	
	END
	