#	$FreeBSD: release/10.1.0/usr.bin/bc/Makefile 265533 2014-05-07 08:06:54Z delphij $
#	$OpenBSD: Makefile,v 1.7 2013/09/19 16:12:00 otto Exp $

PROG=		bc
SRCS=		bc.y scan.l tty.c
CFLAGS+=	-I. -I${.CURDIR}

LDADD+=		-ledit -lcurses
DPADD+=		${LIBEDIT} ${LIBCURSES}

NO_WMISSING_VARIABLE_DECLARATIONS=

FILES+=	bc.library
FILESDIR=${SHAREDIR}/misc

.include <bsd.prog.mk>
