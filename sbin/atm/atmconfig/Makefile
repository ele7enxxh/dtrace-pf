# Copyright (c) 2001-2003
#	Fraunhofer Institute for Open Communication Systems (FhG Fokus).
#	All rights reserved.
# Author: Harti Brandt <brandt@fokus.gmd.de>
#
# $FreeBSD: release/10.1.0/sbin/atm/atmconfig/Makefile 270824 2014-08-29 18:26:55Z ngie $

.include <bsd.own.mk>

PROG=	atmconfig
SRCS=	main.c diag.c natm.c
MAN=	atmconfig.8
# CFLAGS+= -DPATH_HELP='".:/usr/share/doc/atm:/usr/local/share/doc/atm"'

CFLAGS+= -I${.OBJDIR}

.if !defined(RESCUE) && ${MK_BSNMP} != "no"
CFLAGS+=	-DWITH_BSNMP
SRCS+=	oid.h atmconfig_device.c
DPADD+=	${LIBBSNMP}
LDADD+=	-lbsnmp
. if ${MK_DYNAMICROOT} == "no" && ${MK_OPENSSL} != "no"
DPADD+=	${LIBCRYPTO}
LDADD+= -lcrypto
. endif
.endif

CLEANFILES+= oid.h

# XXX - this is verboten
.if ${MACHINE_CPUARCH} == "arm"
WARNS?= 3
.endif

FILES=	atmconfig.help atmconfig_device.help
FILESDIR= /usr/share/doc/atm

SNMP_ATM_DEF= ${.CURDIR}/../../../contrib/ngatm/snmp_atm/atm_tree.def	\
	${.CURDIR}/../../../usr.sbin/bsnmpd/modules/snmp_atm/atm_freebsd.def

oid.h: atm_oid.list ${SNMP_ATM_DEF}
	cat ${SNMP_ATM_DEF} | gensnmptree -e `tail -n +2 ${.CURDIR}/atm_oid.list` \
		> ${.TARGET}

.include <bsd.prog.mk>
