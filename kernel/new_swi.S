@ New_S_handler.S: Kernel startup routine
@
@ Author: Ishan Vashishtha <ivashish@andrew.cmu.edu>
@ Date:   Mon, 20 Oct 2014

	.file	"new_swi.S"
	.text
	.global	New_S_Handler
	@New Swi handler

New_S_Handler:	SUB sp, sp, #4 
	STMFD sp!, {r0-r12, lr} 
	MRS r2, spsr 		
	STR r2, [sp, #14*4]
	MOV r1, sp
	LDR r0, [lr, #-4]
	BIC r0,r0,#0xff000000
	BL C_SWI_Handler
	LDR r2, [sp, #14*4]
	MSR spsr, r2
	LDMFD sp!, {r0-r12, lr}
	ADD sp, sp, #4
	MOVS pc, lr

