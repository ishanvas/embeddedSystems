@ exit.S: exit sycall wrapper
@
@ Authors: Ishan Vashishtha <ivashish@andrew.cmu.edu>
@ Date:    5th October 2014

#include <asm.h>
#include <bits/swi.h>
#include <bits/errno.h>

	.file	"exit.S"
	.text

	.global	exit
	.type exit,%function
exit:
	ORR r0, r0,r0
	swi EXIT_SWI
	
