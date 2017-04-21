 NOTES
 -----

 r1 - r12: General Purpose Registers
 r13: Stack pointer (SP)
 r14: Link Reegister (LR)
 r15: Program Counter (PC)
 xPSR: Current Program Status Register {
 	4 flags {
 		N: Negative (1: Operation returned negative value)
 		Z: Zero (1: Operation returned 0)
 		C: Carry (1: Operation returned a carry)
 		V: Overflow (!: Operation returned value that can
 	}
 }

 Addressing Modes:
 ----------------

 1. Immediate addressing: Data available in instruction.
 > mov r1, #4	; r1 = 4
 2. Register addressing: Data available in register.
 > mov r1, r2 	; r1 = r2
 3. Indirect addressing: Data available in memory.
 > ldr r1, [r2]	; r1 = *r2
 4. Indexed addressing: Extension of indirect.
 > ldr r1, [r2, #4]		; preindexed => r1 << *(r2 + 4)
 > ldr r1, [r2, #4]!	; preindexed with writeback => r1 << *(r2 + 4); r2 << (r2 + 4)
 > ldr r1, [r2], #4		; postindexed => r1 << *r2; r2 << (r2 + 4)

 Instruction Set
 ---------------

 ; throughout the discussion, rd == destination register, rs == source reegister, op == operand
 ; !(op) == 1's compliment of op
 ; (-op) == 2's compliment of op
 
 Load Store:

 mov rd, operand	; operand at max 2 byte (only for mov)
 mov rd, rs 		; rs can be of any size
 ; mov required only when register to register copy required
 movw rs, operand	; move half word; same as mov
 movt rs, #0x49ff	; move to top half; rd << 0x49ff0000
 ; > rd = rs
 
 mvn rd, op 		; move "not"
 ; > rd = !(op)

 ldr rd, = op		; load word (4B == 32b)
 ldrh rd, = op		; load half word (2B == 16b)
 ldrb rd, = op		; load byte (1B == 8b)
 ldrsb rd, = op		; load byte sign extended (signed byte)
 ldrsh rd, = op		; load half word sign extended (signed half word)
 ; > rd = op

 str rs, [rd]		; store word (4B == 32b)
 strh rs, [rd]		; store half word (2B == 16b)
 strb rs, [rd]		; store byte (1B == 8b)
 strsb rs, [rd]		; store byte sign extended (signed byte)
 strsh rs, [rd]		; store half word sign extended (signed half word)
 ; > *rd = rs
 
 Arithmetic: 

 add rd, rn, op 	; rd = (rn + op)
 adc rd, rn, op 	; rd = (rn + op + carry)
 sub rd, rn, op 	; rd = (rn - op)
 sbc rd, rn, op 	; rd = (rn - op - (1 - carry))
 rsb rd, rn, op 	; rd = (op - rn)
 rcb rd, rn, op 	; rd = (op - rn - (1 - carry))
 ; append "s" to any instruction  to make it affect flags
 ; for example, adc does not affect carry flag if carry comes, but adcs will

 cmp rn, op 		; (rn - op) affects flags
 cmn rn, op 		; (rn - (-op)) affects flags
 ; cmp and cmn just update flags depending on operation, result nowhere stored

 msr xpsr, rn  		; xpsr = rn
 mrs rn, xpsr 		; rn = xpsr
 ; only mrs and msr can be used to alter the content of xpsr manually.
 ; for example
 ; mrs r1, xpsr 		; r1 = xpsr
 ; orr r1, 0x20000000	; r1 |= 0x20000000 (set carry flag)
 ; msr xpsr, r1 		; xpsr = r1
 ; xpsr structure: [n][z][c][v][...]

 mul rd, rm, rn 		; rd = (rm * rn) can only store answers upto 32 bits
 umull rdl, rdh, rm, rn ; [rdh:rdl] = (rm * rn) unsigned multiplicaqtion, can store upto 64 bits
 smull rdl, rdh, rm, rn ; [rdh:rdl] = (rm * rn) signed multiplication, can store upto 64 bits

 mla rd, rs1, rs2, rn 	; rd = (rn + (rs1 * rs2))
 mls rd, rs1, rs2, rn 	; rd = (rn - (rs1 * rs2))

 umlal rdl, rdh, rm, rn ; [rdh:rdl] = ([rdh:rdl] + (rm * rn)) unisgned
 smlal rdl, rdh, rm, rn ; [rdh:rdl] = ([rdh:rdl] + (rm * rn)) signed

 Conditionals:

 ; checks status register
 ; suffix to other instructions

 ; carry:
 cs ; if(c == 1)
 cc ; if(c == 0)

 ; negative
 pl ; if(n == 0)
 mi ; if(n == 1)

 ; zero
 eq ; if(z == 1)
 ne ; if(z == 0)

 ; overflow
 vs ; if(v == 1)
 vc	; if(v == 0)

 ; unsigned comparison
 lo ; lower { if(c == 0) }
 hi ; higher { if(c == 1) }
 ls ; lower or same { if(c == 0 || z == 1) }
 hs ; higher or same { if(c == 1) || z == 1) }

 ; signed comparison
 lt ; < { if(n != v && z == 0) }
 gt ; > { if(n == v && z == 0) }
 le ; <= { if(n != v || z == 1) }
 ge ; >= { if(n == v || z == 1) }

 Logical operations:

 bic rd, rn, op 	; rd = (rn && !(op))
 and rd, rn, op 	; rd = (rn && op)
 orr rd, rn, op 	; rd = (rn || op)
 orn rd, rn, op 	; rd = (rn || !(op))
 eor rd, rn, op 	; rd = (rn "xor" op)

 ; shifting does not affect carry flag
 ; to affect, use "s" suffix
 ; when "s" used, removed bit given to carry
 lsl rd, rn, op 	; rd = (rn << op)
 lsr rd, rn, op 	; rd = (rn >> op)
 asr rd, rn, op 	; rd = (rn "arithmetic shift right" by "op bits")
 ; arithmetic shift right => msb retained
 ror rd, rn, op 	; rd = (rn "rotate right" by "op bits")
 ; rotate right => lsb goes to msb
 rrx rd, rn 		; rd = (rn "rotate right through carry" by 1 bit)
