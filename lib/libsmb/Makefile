# $FreeBSD: release/10.1.0/lib/libsmb/Makefile 258125 2013-11-14 09:25:29Z glebius $

.include <bsd.own.mk>

CONTRIBDIR=	${.CURDIR}/../../contrib/smbfs
.PATH: ${CONTRIBDIR}/lib/smb

LIB=	smb
SHLIB_MAJOR=	4
DPADD=	${LIBKICONV}
LDADD=	-lkiconv

SRCS=	rcfile.c ctx.c cfopt.c subr.c nls.c rap.c mbuf.c rq.c file.c \
	print.c \
	kiconv.c \
	nb.c nb_name.c nb_net.c nbns_rq.c

WARNS?=	1
CFLAGS+= -DSMB_CFG_FILE=\"/etc/nsmb.conf\" -I${CONTRIBDIR}/include

.if ${MK_ICONV} != "no"
CFLAGS+= -DHAVE_ICONV=1
.endif

.include <bsd.lib.mk>
