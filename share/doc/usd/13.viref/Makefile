#	From: @(#)Makefile	8.16 (Berkeley) 8/15/94
# $FreeBSD: release/10.1.0/share/doc/usd/13.viref/Makefile 136910 2004-10-24 15:33:08Z ru $

VOLUME=		usd/13.viref
SRCS=		vi.ref-patched
EXTRA=		ex.cmd.roff ref.so set.opt.roff vi.cmd.roff
MACROS=		-me
CLEANFILES=	vi.ref-patched index
TRFLAGS=	-U		# this is to hide warnings only
USE_SOELIM=
USE_TBL=
SRCDIR=		${.CURDIR}/../../../../contrib/nvi/docs/USD.doc/vi.ref

vi.ref-patched: vi.ref
	sed -e 's:^\.so index.so$$:&.\\*[.T]:' ${.ALLSRC} > ${.TARGET}

PRINTERDEVICE?=	ascii
.for _dev in ${PRINTERDEVICE}
EXTRA+=		index.so.${_dev}
CLEANFILES+=	index.so.${_dev}

# Build index.so as a side-effect of building the paper.
index.so.${_dev}: ${SRCS} ${EXTRA:Nindex.so.${_dev}}
	sed -e 's:^\.so index\.so\.\\\*\[\.T\]$$::' vi.ref-patched | \
	    ${ROFF.${_dev}} -U -z
	sed -e 's/MINUSSIGN/-/' \
	    -e 's/DOUBLEQUOTE/""/' \
	    -e "s/SQUOTE/'/" \
	    -e 's/ /__SPACE/g' < index | \
	sort -u '-t	' -k 1,1 -k 2n | awk -f ${SRCDIR}/merge.awk | \
	sed -e 's/__SPACE/ /g' \
	    -e "s/^\\(['\\.]\\)/\\\\\&\\1/" > ${.TARGET}
.endfor

.include <bsd.doc.mk>
