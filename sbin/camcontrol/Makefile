# $FreeBSD: release/10.1.0/sbin/camcontrol/Makefile 268700 2014-07-15 17:26:43Z mav $

PROG=	camcontrol
SRCS=	camcontrol.c util.c
.if !defined(RELEASE_CRUNCH)
SRCS+=	fwdownload.c modeedit.c persist.c progress.c
.else
CFLAGS+= -DMINIMALISTIC
.endif
# This is verboten
.if ${MACHINE_CPUARCH} == "arm"
WARNS?= 3
.endif
DPADD=	${LIBCAM} ${LIBSBUF} ${LIBUTIL}
LDADD=	-lcam -lsbuf -lutil
MAN=	camcontrol.8

.include <bsd.prog.mk>
