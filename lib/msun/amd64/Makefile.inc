# $FreeBSD: release/10.1.0/lib/msun/amd64/Makefile.inc 177768 2008-03-30 21:21:53Z das $

ARCH_SRCS = e_remainder.S e_remainderf.S e_remainderl.S \
	    e_sqrt.S e_sqrtf.S e_sqrtl.S \
	    s_llrint.S s_llrintf.S s_llrintl.S \
	    s_logbl.S s_lrint.S s_lrintf.S s_lrintl.S \
	    s_remquo.S s_remquof.S s_remquol.S \
	    s_rintl.S s_scalbn.S s_scalbnf.S s_scalbnl.S
LDBL_PREC = 64
SYM_MAPS += ${.CURDIR}/amd64/Symbol.map
