/*-
 * Copyright (c) 2009 Joerg Sonnenberger
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD: release/10.1.0/contrib/libarchive/libarchive_fe/err.h 238856 2012-07-28 06:38:44Z mm $
 */

#ifndef LAFE_ERR_H
#define LAFE_ERR_H

#if defined(__GNUC__) && (__GNUC__ > 2 || \
                          (__GNUC__ == 2 && __GNUC_MINOR__ >= 5))
#define __LA_DEAD       __attribute__((__noreturn__))
#else
#define __LA_DEAD
#endif

#if defined(__GNUC__) && (__GNUC__ > 2 || \
			  (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
#define	__LA_PRINTFLIKE(f,a)	__attribute__((__format__(__printf__, f, a)))
#else
#define	__LA_PRINTFLIKE(f,a)
#endif

extern const char *lafe_progname;

void	lafe_warnc(int code, const char *fmt, ...) __LA_PRINTFLIKE(2, 3);
void	lafe_errc(int eval, int code, const char *fmt, ...) __LA_DEAD
		  __LA_PRINTFLIKE(3, 4);

#endif
