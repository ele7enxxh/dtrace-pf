# $FreeBSD: release/10.1.0/usr.bin/atf/atf-sh/tests/Makefile 264483 2014-04-14 23:51:57Z jmmv $

.include <bsd.init.mk>

TESTSDIR=	${TESTSBASE}/usr.bin/atf/atf-sh

ATF=		${.CURDIR:H:H:H:H}/contrib/atf
.PATH:		${ATF}/atf-sh

ATF_TESTS_SH+=	atf_check_test
ATF_TESTS_SH+=	config_test
ATF_TESTS_SH+=	integration_test
ATF_TESTS_SH+=	normalize_test
ATF_TESTS_SH+=	tc_test
ATF_TESTS_SH+=	tp_test

SCRIPTS+=		misc_helpers
SCRIPTSDIR_misc_helpers=${TESTSDIR}
CLEANFILES+=		misc_helpers misc_helpers.tmp
misc_helpers: misc_helpers.sh
	echo '#! /usr/bin/atf-sh' >${.TARGET}.tmp
	cat ${.ALLSRC} >>${.TARGET}.tmp
	chmod +x ${.TARGET}.tmp
	mv ${.TARGET}.tmp ${.TARGET}

.include <bsd.test.mk>
