/*
 * Written by J.T. Conklin, Apr 10, 1995
 * Public domain.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/libc/ia64/gen/flt_rounds.c 132383 2004-07-19 08:17:25Z das $");

#include <float.h>

static const int map[] = {
	1,	/* round to nearest */
	3,	/* round to zero */
	2,	/* round to negative infinity */
	0	/* round to positive infinity */
};

int
__flt_rounds(void)
{
	int x;

	__asm("mov %0=ar.fpsr" : "=r" (x));
        return (map[(x >> 10) & 0x03]);
}
