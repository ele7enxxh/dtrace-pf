# $FreeBSD: release/10.1.0/kerberos5/lib/libkadm5srv/Makefile 236337 2012-05-30 22:21:25Z obrien $

LIB=	kadm5srv
LDFLAGS=	-Wl,--no-undefined
LDADD=	-lcom_err -lhdb -lkrb5 -lroken
DPADD=	${LIBCOM_ERR} ${LIBHDB} ${LIBKRB5} ${LIBROKEN}
VERSION_MAP= ${KRB5DIR}/lib/kadm5/version-script.map
MAN=	kadm5_pwcheck.3

SRCS=	acl.c \
	bump_pw_expire.c \
	chpass_s.c \
	common_glue.c \
	context_s.c \
	create_s.c \
	delete_s.c \
	destroy_s.c \
	ent_setup.c \
	error.c \
	flush_s.c \
	free.c \
	get_princs_s.c \
	get_s.c \
	init_s.c \
	kadm5_err.c \
	kadm5_err.h \
	keys.c \
	log.c \
	marshall.c \
	modify_s.c \
	password_quality.c \
	privs_s.c \
	randkey_s.c \
	rename_s.c \
	server_glue.c \
	set_keys.c \
	set_modifier.c

CFLAGS+=-I${KRB5DIR}/lib/kadm5 -I${KRB5DIR}/lib/asn1 -I${KRB5DIR}/lib/roken -I.

.include <bsd.lib.mk>

.PATH: ${KRB5DIR}/lib/kadm5
