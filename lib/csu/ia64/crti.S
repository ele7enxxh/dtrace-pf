/*-
 * Copyright (c) 2001 Peter Wemm <peter@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: release/10.1.0/lib/csu/ia64/crti.S 216338 2010-12-09 21:31:21Z dim $
 */

#include <machine/asm.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/csu/ia64/crti.S 216338 2010-12-09 21:31:21Z dim $");

/*
 * This file (and its companion crtn.S) form the terminators of the
 * .init and .fini sections.
 */
	.file	"crti.S"

	.section .init,"ax",@progbits
	.global	_init#
	.proc	_init#
_init:
	.regstk	0,2,0,0
	.prologue 12,loc0
	.save	ar.pfs,loc1
	alloc	loc1=ar.pfs,0,2,0,0
	mov	loc0=b0			/* Save return addr */
	.body
	.endp	_init#

	.section .fini,"ax",@progbits
	.global	_fini#
	.proc	_fini#
_fini:
	.regstk	0,2,0,0
	.prologue 12,loc0
	.save	ar.pfs,loc1
	alloc	loc1=ar.pfs,0,2,0,0
	mov	loc0=b0			/* Save return addr */
	.body
	.endp	_fini#
