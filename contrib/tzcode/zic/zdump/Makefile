# $FreeBSD: release/10.1.0/contrib/tzcode/zic/zdump/Makefile 201390 2010-01-02 11:07:44Z ed $

.PATH: ${.CURDIR}/..

PROG=	zdump
MAN=	zdump.8
SRCS=	zdump.c ialloc.c scheck.c

CFLAGS+= -DTM_GMTOFF=tm_gmtoff -DTM_ZONE=tm_zone -DSTD_INSPIRED -DPCTS
CFLAGS+= -DHAVE_LONG_DOUBLE -DTZDIR=\"/usr/share/zoneinfo\" -Demkdir=mkdir
CFLAGS+= -I${.CURDIR}/.. -I${.CURDIR}/../../../lib/libc/stdtime

WARNS?=	2

.include <bsd.prog.mk>
