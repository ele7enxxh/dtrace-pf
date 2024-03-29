#-
# Copyright (c) 2011 Google, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: release/10.1.0/lib/atf/libatf-c/Makefile 270905 2014-08-31 23:09:23Z ngie $

.include <bsd.init.mk>

LIB=		atf-c
PRIVATELIB=	true
SHLIB_MAJOR=	0

ATF=		${.CURDIR:H:H:H}/contrib/atf
.PATH:		${ATF}
.PATH:		${ATF}/atf-c
.PATH:		${ATF}/atf-c/detail

CFLAGS+=	-I${ATF}
CFLAGS+=	-I${.CURDIR}
CFLAGS+=	-I.

SRCS=		build.c \
		check.c \
		config.c \
		dynstr.c \
		env.c \
		error.c \
		fs.c \
		list.c \
		map.c \
		process.c \
		sanity.c \
		text.c \
		user.c \
		utils.c \
		tc.c \
		tp.c \
		tp_main.c

INCS=		build.h \
		check.h \
		config.h \
		defs.h \
		error.h \
		error_fwd.h \
		macros.h \
		tc.h \
		tp.h \
		utils.h
INCSDIR=	${INCLUDEDIR}/atf-c

INCS+=		atf-c.h
INCSDIR_atf-c.h= ${INCLUDEDIR}

MAN=		atf-c-api.3

.if ${MK_TESTS} != "no"
SUBDIR=		tests
.endif

.include "../common.mk"
.include <bsd.lib.mk>
