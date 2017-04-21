	AREA RESET,DATA,READONLY
	EXPORT __Vectors
	
__Vectors
	DCD 0x40001000
	DCD Reset_Handler
	
	AREA gcd,CODE,READONLY
	EXPORT Reset_Handler
	ENTRY
	
Reset_Handler
	LDR R0,=num1
	LDR R1,=num2
	LDR R2,[R0]
	LDR R3,[R1]
	LDR R5,=gcdresult
UP	CMP R2,R3
	BEQ EXIT
	SUBGT R2,R2,R3
	SUBLT R3,R3,R2
	B UP
EXIT
	STR R2,[R5]

STOP
	B STOP

num1
	DCD 0x12
num2
	DCD 0x34
	
	AREA gcdres,DATA,READWRITE
gcdresult
	DCD 0

	END