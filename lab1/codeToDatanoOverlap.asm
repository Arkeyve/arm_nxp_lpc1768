				AREA RESET, DATA, READONLY
				EXPORT __Vectors
		
__Vectors		DCD 0x40001000
				DCD Reset_Handler
			
				AREA code_to_data, CODE, READONLY
				EXPORT Reset_Handler
				ENTRY
			
Reset_Handler	LDR r1, = src
				LDR r2, = dst
				MOV r4, #10
UP				LDR r3, [r1], #4
				STR r3, [r2], #4
				SUB r4, #1
				CMP r4, #0
				BNE UP
STOP			B STOP

src				DCD 0xDEADBEEF, 0x12345678, 0x87654321, 0x13578642, 0x24687531, 0xABCDEF12, 0x21fedcba, 0x1a2b3c4d, 0xd4c3b2a1, 0xa8b7c6d5

				AREA dataArea,DATA,READWRITE
dst				DCD 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

				END