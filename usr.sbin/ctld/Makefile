# $FreeBSD: release/10.1.0/usr.sbin/ctld/Makefile 270970 2014-09-02 16:14:16Z ngie $

PROG=		ctld
SRCS=		ctld.c discovery.c kernel.c keys.c log.c login.c parse.y pdu.c token.l y.tab.h
CFLAGS+=	-I${.CURDIR}
CFLAGS+=	-I${.CURDIR}/../../sys
CFLAGS+=	-I${.CURDIR}/../../sys/cam/ctl
CFLAGS+=	-I${.CURDIR}/../../sys/dev/iscsi
#CFLAGS+=	-DICL_KERNEL_PROXY
MAN=		ctld.8 ctl.conf.5

DPADD=		${LIBBSDXML} ${LIBCAM} ${LIBCRYPTO} ${LIBL} ${LIBSBUF} ${LIBSSL} ${LIBUTIL}
LDADD=		-lbsdxml -lcam -lcrypto -ll -lsbuf -lssl -lutil

YFLAGS+=	-v
CLEANFILES=	y.tab.c y.tab.h y.output

WARNS=		6
NO_WMISSING_VARIABLE_DECLARATIONS=

.include <bsd.prog.mk>
