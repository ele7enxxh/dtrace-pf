# $FreeBSD: release/10.1.0/gnu/usr.bin/cc/cc/Makefile 234782 2012-04-29 09:32:44Z kib $

.include <bsd.own.mk>

.include "../Makefile.inc"
.include "../Makefile.fe"

PROG=	gcc
MAN=	gcc.1
SRCS+=  gccspec.c

.if ${MK_SHARED_TOOLCHAIN} == "no"
NO_SHARED?=yes
.endif

MLINKS=	gcc.1 g++.1
.if ${MK_CLANG_IS_CC} == "no"
LINKS=	${BINDIR}/gcc ${BINDIR}/cc
MLINKS+= gcc.1 cc.1 gcc.1 c++.1 gcc.1 CC.1
.endif

.include <bsd.prog.mk>
