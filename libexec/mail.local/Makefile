#	@(#)Makefile	8.1 (Berkeley) 7/19/93
# $FreeBSD: release/10.1.0/libexec/mail.local/Makefile 246880 2013-02-16 20:17:31Z dim $

SENDMAIL_DIR=${.CURDIR}/../../contrib/sendmail
.PATH:	${SENDMAIL_DIR}/mail.local

PROG=	mail.local
SRCS=	mail.local.c
MAN=	mail.local.8
CFLAGS+=-I${SENDMAIL_DIR}/include -I.

WARNS?=	2
WFORMAT=0

LIBSMDIR=	${.OBJDIR}/../../lib/libsm
LIBSM=		${LIBSMDIR}/libsm.a

DPADD=	${LIBSM}
LDADD=	${LIBSM}

SRCS+=	sm_os.h
CLEANFILES+=sm_os.h

# User customizations to the sendmail build environment
CFLAGS+=${SENDMAIL_CFLAGS}
DPADD+=${SENDMAIL_DPADD}
LDADD+=${SENDMAIL_LDADD}
LDFLAGS+=${SENDMAIL_LDFLAGS}

sm_os.h:
	ln -sf ${SENDMAIL_DIR}/include/sm/os/sm_os_freebsd.h sm_os.h

.include <bsd.prog.mk>
