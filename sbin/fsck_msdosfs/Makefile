#	$NetBSD: Makefile,v 1.6 1997/05/08 21:11:11 gwr Exp $
# $FreeBSD: release/10.1.0/sbin/fsck_msdosfs/Makefile 241807 2012-10-21 12:01:19Z uqs $

FSCK=	${.CURDIR}/../fsck
.PATH: ${FSCK}

PROG=	fsck_msdosfs
MAN=	fsck_msdosfs.8
SRCS=	main.c check.c boot.c fat.c dir.c fsutil.c

CFLAGS+= -I${FSCK}

.include <bsd.prog.mk>
