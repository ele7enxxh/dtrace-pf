# $FreeBSD: release/10.1.0/usr.bin/mkcsmapper_static/Makefile 219019 2011-02-25 00:04:39Z gabor $

.PATH: ${.CURDIR}/../../lib/libc/iconv ${.CURDIR}/../mkcsmapper

PROG=	mkcsmapper_static
SRCS=	citrus_bcs.c citrus_db_factory.c citrus_db_hash.c \
	citrus_lookup_factory.c citrus_pivot_factory.c
NO_MAN=	yes
NO_SHARED= yes

build-tools: mkcsmapper_static

.include "${.CURDIR}/../mkcsmapper/Makefile.inc"
.include <bsd.prog.mk>
