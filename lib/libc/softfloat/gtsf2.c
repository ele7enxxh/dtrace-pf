/* $NetBSD: gtsf2.c,v 1.1 2000/06/06 08:15:06 bjh21 Exp $ */

/*
 * Written by Ben Harris, 2000.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/libc/softfloat/gtsf2.c 129203 2004-05-14 12:13:06Z cognet $");

flag __gtsf2(float32, float32);

flag
__gtsf2(float32 a, float32 b)
{

	/* libgcc1.c says a > b */
	return float32_lt(b, a);
}
