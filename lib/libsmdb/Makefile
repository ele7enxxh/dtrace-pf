# $FreeBSD: release/10.1.0/lib/libsmdb/Makefile 201381 2010-01-02 09:58:07Z ed $

SENDMAIL_DIR=${.CURDIR}/../../contrib/sendmail
.PATH:	${SENDMAIL_DIR}/libsmdb

CFLAGS+=-I${SENDMAIL_DIR}/src -I${SENDMAIL_DIR}/include -I.
CFLAGS+=-DNEWDB -DNOT_SENDMAIL

# User customizations to the sendmail build environment
CFLAGS+=${SENDMAIL_CFLAGS}

WARNS?=	3

LIB=	smdb

SRCS+=	sm_os.h
SRCS+=	smdb.c smdb1.c smdb2.c smndbm.c
CLEANFILES+=sm_os.h

INTERNALLIB=

sm_os.h:
	ln -sf ${SENDMAIL_DIR}/include/sm/os/sm_os_freebsd.h sm_os.h

.include <bsd.lib.mk>
