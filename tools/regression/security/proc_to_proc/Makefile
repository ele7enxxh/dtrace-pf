# $FreeBSD: release/10.1.0/tools/regression/security/proc_to_proc/Makefile 139103 2004-12-21 08:47:35Z ru $

PROG=	testuid
NO_MAN=

ADDITIONAL_FLAGS=-DSETSUGID_SUPPORTED
#ADDITIONAL_FLAGS=	-DSETSUGID_SUPPORTED_BUT_NO_LIBC_STUB
ADDITIONAL_FLAGS+=	-DCHECK_CRED_SET
CFLAGS+=	${ADDITIONAL_FLAGS}
SRCS=	testuid.c scenario.c

.include <bsd.prog.mk>
