# $FreeBSD: release/10.1.0/lib/libmp/tests/Makefile 269534 2014-08-04 22:10:07Z ngie $

TESTSDIR=	${TESTSBASE}/lib/libmp

TAP_TESTS_C+=	legacy_test

DPADD+=		${LIBCRYPTO} ${LIBMP}
LDADD+=		-lcrypto -lmp

.include <bsd.test.mk>
