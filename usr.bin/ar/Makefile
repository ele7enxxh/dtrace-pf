# $FreeBSD: release/10.1.0/usr.bin/ar/Makefile 249657 2013-04-19 19:45:00Z ed $

.include <bsd.own.mk>

PROG=	ar
SRCS=	ar.c acplex.l acpyacc.y read.c util.c write.c y.tab.h

DPADD=	${LIBARCHIVE} ${LIBELF} ${LIBZ}
LDADD=	-larchive -lelf -lz

NO_WMISSING_VARIABLE_DECLARATIONS=

CFLAGS+=-I. -I${.CURDIR}

.if ${MK_SHARED_TOOLCHAIN} == "no"
NO_SHARED?=	yes
.endif
LINKS=	${BINDIR}/ar ${BINDIR}/ranlib
MLINKS= ar.1 ranlib.1

.include <bsd.prog.mk>
