# $FreeBSD: release/10.1.0/sys/modules/opensolaris/Makefile 253996 2013-08-06 15:51:56Z avg $

.PATH: ${.CURDIR}/../../cddl/compat/opensolaris/kern

KMOD=		opensolaris
SRCS=		opensolaris.c		\
		opensolaris_cmn_err.c	\
		opensolaris_kmem.c	\
		opensolaris_misc.c	\
		opensolaris_sunddi.c

_A=${.CURDIR}/../../cddl/contrib/opensolaris/common/atomic
.if exists(${_A}/${MACHINE_CPUARCH}/opensolaris_atomic.S)
.PATH:	${_A}/${MACHINE_CPUARCH}
SRCS+=		opensolaris_atomic.S
.elif exists(${_A}/${MACHINE_ARCH}/opensolaris_atomic.S)
.PATH:	${_A}/${MACHINE_ARCH}
SRCS+=		opensolaris_atomic.S
.else
SRCS+=		opensolaris_atomic.c
.endif

CFLAGS+=	-I${.CURDIR}/../../cddl/compat/opensolaris		\
		-I${.CURDIR}/../../cddl/contrib/opensolaris/uts/common	\
		-I${.CURDIR}/../..

IGNORE_PRAGMA=	1

.include <bsd.kmod.mk>

CFLAGS+=	-include ${.CURDIR}/../../cddl/compat/opensolaris/sys/debug_compat.h
