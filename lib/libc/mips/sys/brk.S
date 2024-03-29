/*	$NetBSD: brk.S,v 1.16 2003/08/07 16:42:17 agc Exp $	*/

/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Ralph Campbell.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <machine/asm.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/libc/mips/sys/brk.S 209521 2010-06-25 05:36:36Z jchandra $");
#include "SYS.h"

#if defined(LIBC_SCCS) && !defined(lint)
	ASMSTR("from: @(#)brk.s	8.1 (Berkeley) 6/4/93")
	ASMSTR("$NetBSD: brk.S,v 1.16 2003/08/07 16:42:17 agc Exp $")
#endif /* LIBC_SCCS and not lint */

	.globl	_C_LABEL(minbrk)
	.globl	_C_LABEL(__curbrk)
	.globl	_C_LABEL(_end)

	.data
_C_LABEL(minbrk):
	PTR_WORD	_C_LABEL(_end)

	.text
LEAF(__sys_brk)
	WEAK_ALIAS(brk, __sys_brk)
	WEAK_ALIAS(_brk, __sys_brk)
	PIC_PROLOGUE(__sys_brk)
	PTR_LA	v0, _C_LABEL(minbrk)
	PTR_L	v0, 0(v0)
	bgeu	a0, v0, 1f
	move	a0, v0		# dont allow break < minbrk
1:
	li	v0, SYS_break
	syscall
	bne	a3, zero, 2f
	PTR_LA	t0, _C_LABEL(__curbrk)
	PTR_S	a0, 0(t0)
	move	v0, zero
	PIC_RETURN()
2:
	PIC_TAILCALL(__cerror)
END(__sys_brk)
