/*
 * Copyright (c) 2003 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
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
 *
 * $FreeBSD: release/10.1.0/lib/libkse/arch/ia64/include/atomic_ops.h 174112 2007-11-30 17:20:29Z deischen $
 */

#ifndef	_ATOMIC_OPS_H_
#define	_ATOMIC_OPS_H_

static inline void
atomic_swap_int(volatile int *dst, int val, int *res)
{
	__asm("xchg4	%0=[%2],%1" : "=r"(*res) : "r"(val), "r"(dst));
}

static inline void
atomic_swap_long(volatile long *dst, long val, long *res)
{
	__asm("xchg8	%0=[%2],%1" : "=r"(*res) : "r"(val), "r"(dst));
}

#define	atomic_swap_ptr(d,v,r)		\
	atomic_swap_long((volatile long *)d, (long)v, (long *)r)

#endif /* _ATOMIC_OPS_H_ */
