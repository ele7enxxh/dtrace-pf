# $FreeBSD: release/10.1.0/sys/modules/dtrace/fasttrap/Makefile 268734 2014-07-16 02:02:46Z pfg $

.PATH:	${.CURDIR}/../../../cddl/contrib/opensolaris/uts/common/dtrace

KMOD=		fasttrap
SRCS=		fasttrap.c fasttrap_isa.c opt_compat.h
SRCS+=		vnode_if.h

CFLAGS+=	-I${.CURDIR}/../../../cddl/compat/opensolaris \
		-I${.CURDIR}/../../../cddl/contrib/opensolaris/uts/common \
		-I${.CURDIR}/../../..

.if ${MACHINE_CPUARCH} == "amd64" ||  ${MACHINE_CPUARCH} == "i386"
CFLAGS+=	-I${.CURDIR}/../../../cddl/contrib/opensolaris/uts/intel
.PATH:		${.CURDIR}/../../../cddl/contrib/opensolaris/uts/intel/dtrace
.elif ${MACHINE_CPUARCH} == "powerpc"
CFLAGS+=	-I${.CURDIR}/../../../cddl/contrib/opensolaris/uts/powerpc
.PATH:		${.CURDIR}/../../../cddl/contrib/opensolaris/uts/powerpc/dtrace
.endif

.PATH:		${.CURDIR}/../../../cddl/contrib/opensolaris/common/unicode
SRCS+=		u8_textprep.c

CFLAGS+=	-DSMP

.include <bsd.kmod.mk>

CFLAGS+=	-include ${.CURDIR}/../../../cddl/compat/opensolaris/sys/debug_compat.h
CWARNFLAGS+=	-Wno-cast-qual
CWARNFLAGS+=	-Wno-unused
