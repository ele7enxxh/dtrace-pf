# $FreeBSD: release/10.1.0/lib/libproc/Makefile 268775 2014-07-16 21:04:31Z dim $

.include <bsd.own.mk>

LIB=	proc

SRCS=	proc_bkpt.c		\
	proc_create.c		\
	proc_regs.c		\
	proc_sym.c		\
	proc_rtld.c		\
	proc_util.c

INCS=	libproc.h

CFLAGS+=	-I${.CURDIR}

.if ${MK_LIBCPLUSPLUS} != "no"
LDADD+=		-lcxxrt
DPADD+=		${LIBCXXRT}
.elif ${MK_GNUCXX} != "no"
LDADD+=		-lsupc++
DPADD+=		${LIBSTDCPLUSPLUS}
.else
CFLAGS+=	-DNO_CXA_DEMANGLE
.endif

SHLIB_MAJOR=	2

WITHOUT_MAN=

.include <bsd.lib.mk>
