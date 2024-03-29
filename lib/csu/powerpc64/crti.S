/*-
 * Copyright 2001 David E. O'Brien
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <machine/asm.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/csu/powerpc64/crti.S 218824 2011-02-18 21:44:53Z nwhitehorn $");

	.section .init,"ax",@progbits
	.align	2
	.globl	_init
	.section ".opd","aw"
	.align	3
_init:
	.quad	.L._init,.TOC.@tocbase,0
	.previous
	.type	_init,@function

	.align 4
.L._init:
	stdu 1,-48(1)
	mflr 0
	std 0,64(1)

	.section .fini,"ax",@progbits
	.align	2
	.globl	_fini
	.section ".opd","aw"
	.align	3
_fini:
	.quad	.L._fini,.TOC.@tocbase,0
	.previous
	.type	_fini,@function

	.align 4
.L._fini:
	stdu 1,-48(1)
	mflr 0
	std 0,64(1)

	.section .note.GNU-stack,"",%progbits
