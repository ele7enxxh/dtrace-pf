/* $NetBSD: nedf2.c,v 1.1 2000/06/06 08:15:07 bjh21 Exp $ */

/*
 * Written by Ben Harris, 2000.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/libc/softfloat/nedf2.c 129203 2004-05-14 12:13:06Z cognet $");

flag __nedf2(float64, float64);

flag
__nedf2(float64 a, float64 b)
{

	/* libgcc1.c says a != b */
	return !float64_eq(a, b);
}
