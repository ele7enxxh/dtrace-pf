# $FreeBSD: release/10.1.0/sys/modules/if_gre/Makefile 151350 2005-10-14 23:30:17Z yar $

.PATH: ${.CURDIR}/../../net ${.CURDIR}/../../netinet ${.CURDIR}/../../netinet6

KMOD=	if_gre
SRCS=	if_gre.c ip_gre.c opt_inet.h opt_inet6.h opt_atalk.h

.if !defined(KERNBUILDDIR)
opt_inet.h:
	echo "#define INET 1" > ${.TARGET}

opt_inet6.h:
	echo "#define INET6 1" > ${.TARGET}

opt_atalk.h:
	echo "#define NETATALK 1" > ${.TARGET}
.endif

.include <bsd.kmod.mk>
