#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: release/10.1.0/usr.sbin/lpr/lprm/Makefile 98702 2002-06-23 20:57:18Z gad $

.PATH: ${.CURDIR}/../common_source

BINDIR=	/usr/bin

PROG=	lprm
BINOWN=	root
BINGRP=	daemon
BINMODE= 6555

CFLAGS+= -I${.CURDIR}/../common_source

DPADD=	${LIBLPR}
LDADD=	${LIBLPR}

.include <bsd.prog.mk>
