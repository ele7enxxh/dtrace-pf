# $FreeBSD: release/10.1.0/usr.bin/iscsictl/Makefile 270907 2014-09-01 01:39:20Z ngie $

PROG=		iscsictl
SRCS=		iscsictl.c periphs.c parse.y token.l y.tab.h
CFLAGS+=	-I${.CURDIR}
CFLAGS+=	-I${.CURDIR}/../../sys/dev/iscsi
MAN=		iscsictl.8

DPADD=		${LIBCAM} ${LIBUTIL} 
LDADD=		-lcam -lutil

YFLAGS+=	-v
LFLAGS+=	-i
CLEANFILES=	y.tab.c y.tab.h y.output

WARNS=		6
NO_WMISSING_VARIABLE_DECLARATIONS=

.include <bsd.prog.mk>
