#	@(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: release/10.1.0/usr.bin/systat/Makefile 262643 2014-03-01 03:09:16Z brooks $

.include <bsd.own.mk>

PROG=	systat
SRCS=	cmds.c cmdtab.c devs.c fetch.c iostat.c keyboard.c main.c \
	netcmds.c netstat.c pigs.c swap.c icmp.c \
	mode.c ip.c tcp.c \
	vmstat.c convtbl.c ifcmds.c ifstat.c

.if ${MK_INET6_SUPPORT} != "no"
SRCS+=	icmp6.c ip6.c
CFLAGS+= -DINET6
.endif

WARNS?=	0

DPADD=	${LIBM} ${LIBDEVSTAT} ${LIBKVM}
LDADD=	-lm -ldevstat -lkvm

.if ${MK_NCURSESW} == "no"
DPADD+=		${LIBNCURSES}
LDADD+=		-lncurses
.else
CFLAGS+=	-DUSE_WIDECHAR
DPADD+=		${LIBNCURSESW}
LDADD+=		-lncursesw
.endif

.include <bsd.prog.mk>
