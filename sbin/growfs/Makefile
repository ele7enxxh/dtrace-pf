#	@(#)Makefile	8.8 (Berkeley) 6/21/2000
#
# $TSHeader: src/sbin/growfs/Makefile,v 1.4 2000/12/05 19:45:24 tomsoft Exp $
# $FreeBSD: release/10.1.0/sbin/growfs/Makefile 264996 2014-04-27 01:15:10Z jmmv $
#

#GFSDBG=

.include <bsd.own.mk>

.PATH:	${.CURDIR}/../mount

PROG=   growfs
SRCS=   growfs.c getmntopts.c
MAN=	growfs.8
CFLAGS+=-I${.CURDIR}/../mount

.if defined(GFSDBG)
SRCS+=  debug.c
.endif  

DPADD=	${LIBUTIL}
LDADD=	-lutil

.if ${MK_TESTS} != "no"
SUBDIR+=    tests
.endif

.include <bsd.prog.mk>      
