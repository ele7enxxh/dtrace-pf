# $FreeBSD: release/10.1.0/share/i18n/esdb/Makefile 255651 2013-09-17 20:09:25Z jilles $
# $NetBSD: Makefile,v 1.14 2007/04/01 18:52:30 tnozaki Exp $

FILESDIR= ${ESDBDIR}

SUBDIR=	APPLE AST BIG5 CP DEC EUC EBCDIC GB GEORGIAN ISO-2022 ISO-8859 \
	ISO646 KAZAKH KOI MISC TCVN UTF

FILES+=	esdb.dir esdb.dir.db esdb.alias esdb.alias.db
CLEANFILES= ${FILES}

esdb.dir: ${SUBDIR}
	newfile=$$(for i in ${SUBDIR}; do \
		cat $$i/esdb.dir.$$i; \
	done); \
	[ "$$newfile" = "$$(cat ${.TARGET} 2>/dev/null)" ] || \
		printf '%s\n' "$$newfile" >${.TARGET}
esdb.dir.db: esdb.dir
	${MKESDB} -m -o ${.TARGET} ${.ALLSRC}

esdb.alias: ${SUBDIR}
	newfile=$$(for i in ${SUBDIR}; do \
		cat $$i/esdb.alias.$$i; \
	done); \
	[ "$$newfile" = "$$(cat ${.TARGET} 2>/dev/null)" ] || \
		printf '%s\n' "$$newfile" >${.TARGET}
esdb.alias.db: esdb.alias
	${MKESDB} -m -o ${.TARGET} ${.ALLSRC}

all: ${FILES}
realall: ${FILES}

.include "./Makefile.inc"
.include <bsd.prog.mk>
