#	@(#)Makefile	8.2 (Berkeley) 4/19/94
# $FreeBSD: release/10.1.0/games/fortune/datfiles/Makefile 248200 2013-03-12 12:35:02Z jhb $

DB=	fortunes freebsd-tips murphy startrek zippy

# TO AVOID INSTALLING THE POTENTIALLY OFFENSIVE FORTUNES, COMMENT OUT THE
# NEXT LINE.
DB+=	limerick murphy-o gerrold.limerick

BLDS=	${DB:S/$/.dat/}
FILES=	${DB} ${BLDS}
CLEANFILES+=${BLDS}

FILESDIR=	${SHAREDIR}/games/fortune

.for f in ${DB}
$f.dat: $f
	PATH=$$PATH:/usr/games:${.OBJDIR}/../strfile \
	    strfile -Cs ${.ALLSRC} ${.TARGET}
.endfor

.include <bsd.prog.mk>
