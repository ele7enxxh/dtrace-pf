# $FreeBSD: release/10.1.0/usr.sbin/tcpdmatch/Makefile 201390 2010-01-02 11:07:44Z ed $

.include <bsd.own.mk>

.PATH: ${.CURDIR}/../../contrib/tcp_wrappers

PROG=	tcpdmatch
MAN=	tcpdmatch.8
SRCS=	tcpdmatch.c fakelog.c inetcf.c scaffold.c

CFLAGS+=-DREAL_DAEMON_DIR=\"/usr/libexec\" \
	-DSEVERITY=LOG_INFO -DRFC931_TIMEOUT=10
.if ${MK_INET6_SUPPORT} != "no"
CFLAGS+=-DINET6
.endif

WARNS?=	0

DPADD=	${LIBWRAP}
LDADD=	-lwrap

.include <bsd.prog.mk>
