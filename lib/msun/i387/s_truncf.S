/*
 * Based on code written by J.T. Conklin <jtc@NetBSD.org>.
 * Public domain.
 */

#include <machine/asm.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/msun/i387/s_truncf.S 217108 2011-01-07 16:13:12Z kib $")

ENTRY(truncf)
	pushl	%ebp
	movl	%esp,%ebp
	subl	$8,%esp

	fstcw	-4(%ebp)		/* store fpu control word */
	movw	-4(%ebp),%dx
	orw	$0x0c00,%dx		/* round towards -oo */
	movw	%dx,-8(%ebp)
	fldcw	-8(%ebp)		/* load modfied control word */

	flds	8(%ebp)			/* round */
	frndint

	fldcw	-4(%ebp)		/* restore original control word */

	leave
	ret
END(truncf)

	.section .note.GNU-stack,"",%progbits
