# $FreeBSD: release/10.1.0/kerberos5/libexec/kcm/Makefile 255455 2013-09-10 18:40:43Z des $

PROG=	kcm
MAN=	kcm.8

SRCS=	acl.c \
	acquire.c \
	cache.c \
	client.c \
	config.c \
	connect.c \
	events.c \
	glue.c \
	log.c \
	main.c \
	protocol.c \
	sessions.c \
	renew.c

CFLAGS+=-I${KRB5DIR}/lib/krb5 -I${KRB5DIR}/lib/asn1 -I${KRB5DIR}/lib/roken \
	-I${KRB5DIR}/kcm -I${KRB5DIR}/lib/ipc ${LDAPCFLAGS}
DPADD=	${LIBHDB} ${LIBKRB5} ${LIBROKEN} ${LIBASN1} ${LIBHEIMNTLM} \
	${LIBHEIMIPCS} ${LIBCRYPTO} ${LIBCRYPT} ${LIBVERS} ${LDAPDPADD}
LDADD=	-lhdb -lkrb5 -lroken -lasn1 -lheimntlm -lheimipcs \
	-lcrypto -lcrypt ${LIBVERS} ${LDAPLDADD}
USEPRIVATELIB= heimipcs
LDFLAGS=${LDAPLDFLAGS}

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kcm
