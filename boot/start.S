#include <common/asm.h>

.extern arm64_elX_to_el1
.extern boot_cpu_stack
.extern secondary_boot_flag
.extern clear_bss_flag
.extern init_c

BEGIN_FUNC(_start)
	mrs	x8, mpidr_el1	/* mrs: state register to gr	msr: gr to state register */
	and	x8, x8,	#0xFF
	cbz	x8, primary	/* CBZ  比较（Compare），如果结果为零（Zero）就转移（只能跳到后面的指令） */

  /* hang all secondary processors before we intorduce multi-processors */
secondary_hang:
	bl secondary_hang

primary:

	/* Turn to el1 from other exception levels. */
	bl 	arm64_elX_to_el1

	/* Prepare stack pointer and jump to C. */
	adr 	x0, boot_cpu_stack
	add 	x0, x0, #0x1000
	mov 	sp, x0

	bl 	init_c

	/* Should never be here */
	b	.	  /* B.相当于while(1); . is actually the address of this line of code */
END_FUNC(_start)
