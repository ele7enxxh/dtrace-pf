# $FreeBSD: release/10.1.0/secure/lib/libssl/Makefile 246772 2013-02-13 23:07:20Z jkim $

LIB=		ssl
SHLIB_MAJOR=	7

NO_LINT=

.if exists(Makefile.man)
.include "Makefile.man"
.endif
.include "../libcrypto/Makefile.inc"

SRCS=	bio_ssl.c d1_both.c d1_clnt.c d1_enc.c d1_lib.c d1_meth.c d1_pkt.c \
	d1_srtp.c d1_srvr.c s23_clnt.c s23_lib.c s23_meth.c s23_pkt.c \
	s23_srvr.c s2_clnt.c s2_enc.c s2_lib.c s2_meth.c s2_pkt.c s2_srvr.c \
	s3_both.c s3_cbc.c s3_clnt.c s3_enc.c s3_lib.c s3_meth.c s3_pkt.c \
	s3_srvr.c ssl_algs.c ssl_asn1.c ssl_cert.c ssl_ciph.c ssl_err.c \
	ssl_err2.c ssl_lib.c ssl_rsa.c ssl_sess.c ssl_stat.c ssl_txt.c \
	t1_clnt.c t1_enc.c t1_lib.c t1_meth.c t1_reneg.c t1_srvr.c tls_srp.c

INCS=	dtls1.h kssl.h srtp.h ssl.h ssl2.h ssl23.h ssl3.h tls1.h
INCSDIR=${INCLUDEDIR}/openssl

DPADD=	${LIBCRYPTO}
LDADD=	-lcrypto

.include <bsd.lib.mk>

.PATH:	${LCRYPTO_SRC}/ssl \
	${.CURDIR}/man
