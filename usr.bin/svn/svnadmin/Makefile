# $FreeBSD: release/10.1.0/usr.bin/svn/svnadmin/Makefile 251886 2013-06-18 02:53:45Z peter $

NO_MAN=	svnadmin.1

.include "${.CURDIR}/../Makefile.inc"

.PATH:	${SVNDIR}/svnadmin

PROG=	svn${SVNLITE}admin

SRCS=	svnadmin.c

CFLAGS+=-I${SVNDIR}/include -I${SVNDIR} -I${.CURDIR}/.. \
	-I${.CURDIR}/../lib/libapr \
	-I${APR}/include/arch/unix \
	-I${APR}/include \
	-I${.CURDIR}/../lib/libapr_util \
	-I${APRU}/include/private \
	-I${APRU}/include

LDADD=	-L${LIBSVN_REPOSDIR} -lsvn_repos \
	-L${LIBSVN_FSDIR} -lsvn_fs \
	-L${LIBSVN_FS_FSDIR} -lsvn_fs_fs \
	-L${LIBSVN_FS_UTILDIR} -lsvn_fs_util \
	-L${LIBSVN_DELTADIR} -lsvn_delta \
	-L${LIBSVN_SUBRDIR} -lsvn_subr \
	-L${LIBAPR_UTILDIR} -lapr-util \
	-lbsdxml \
	-L${LIBAPRDIR} -lapr \
	-L${LIBSQLITEDIR} -lsqlite3 \
	-lz -lcrypt

DPADD=	${LIBSVN_REPOS} ${LIBSVN_FS} ${LIBSVN_FS_FS} ${LIBSVN_FS_UTIL} \
	${LIBSVN_DELTA} ${LIBSVN_SUBR} ${LIBAPR_UTIL} \
	${LIBBSDXML} ${LIBAPR} ${LIBSQLITE} ${LIBZ} ${LIBCRYPT}

.include <bsd.prog.mk>
