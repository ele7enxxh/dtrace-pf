# $FreeBSD: release/10.1.0/gnu/usr.bin/gdb/doc/Makefile 211243 2010-08-12 20:46:49Z will $

INFO=	gdb gdbint stabs
INFOSECTION= "Software development"

MAKEINFOFLAGS+= --no-validate
MAKEINFOFLAGS+= -I ${CNTRB_GDB}/gdb/doc
MAKEINFOFLAGS+= -I ${CNTRB_GDB}/gdb/mi
MAKEINFOFLAGS+= -I ${CNTRB_RL}/doc

gdb.info: gdb.texinfo gdb-cfg.texi GDBvn.texi annotate.texinfo fdl.texi	\
	gpl.texi inc-hist.texinfo rluser.texinfo

gdbint.info: gdbint.texinfo gdb-cfg.texi fdl.texi observer.texi

gdb-cfg.texi: all-cfg.texi
	ln -sf ${.ALLSRC} ${.TARGET}

GDBvn.texi: version.in
	echo "@set GDBVN `sed q ${.ALLSRC}`" > ${.TARGET}

inc-hist.texinfo: hsuser.texi inc-hist.diff
	cp -f ${.ALLSRC:M*.texi} ${.TARGET}
	patch < ${.ALLSRC:M*.diff}

rluser.texinfo: rluser.texi
	cp -f ${.ALLSRC:M*.texi} ${.TARGET}

CLEANFILES= gdb-cfg.texi GDBvn.texi inc-hist.texinfo inc-hist.texinfo.orig \
	rluser.texinfo

.include <bsd.info.mk>
