#	@(#)Makefile	8.1 (Berkeley) 6/8/93
# $FreeBSD: release/10.1.0/share/doc/psd/01.cacm/Makefile 136910 2004-10-24 15:33:08Z ru $

VOLUME=		psd/01.cacm
SRCS=		stubs p.mac p1 p2 p3 p4 p5 p6
EXTRA=		ref.bib
MACROS=		-ms
USE_REFER=
USE_TBL=
CLEANFILES=	stubs

stubs:
	@(echo .R1; echo database ${.CURDIR}/ref.bib; \
	    echo accumulate; echo .R2) > ${.TARGET}

.include <bsd.doc.mk>
