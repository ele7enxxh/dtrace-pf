# @(#)Makefile    8.8 (Berkeley) 6/21/2000
#
# $TSHeader: src/sbin/ffsinfo/Makefile,v 1.3 2000/12/05 19:45:10 tomsoft Exp $
# $FreeBSD: release/10.1.0/sbin/ffsinfo/Makefile 227407 2011-11-09 21:01:50Z ed $
#

GROWFS= ${.CURDIR}/../growfs
.PATH: ${GROWFS}

PROG=	ffsinfo
SRCS=	ffsinfo.c debug.c
MAN=	ffsinfo.8

WARNS?=	1
CFLAGS+=-DFS_DEBUG -I${GROWFS}
DPADD=	${LIBUFS}
LDADD=	-lufs

.include <bsd.prog.mk>
