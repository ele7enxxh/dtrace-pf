# $FreeBSD: release/10.1.0/sys/modules/iwnfw/Makefile.inc 208591 2010-05-27 12:54:42Z rpaulo $
#
# Common rules for building firmware.  Note this gets auto-included
# by the subdir Makefile's as a consequence of included bsd.kmod.mk.

_FIRM=	${IMG}.fw

CLEANFILES+=	${_FIRM}

FIRMWS=	${_FIRM}:${KMOD}

#
# Note that a license ack is not needed for iwn.
#
#FIRMWARE_LICENSE=

${_FIRM}: ${.CURDIR}/../../../contrib/dev/iwn/${_FIRM}.uu
	uudecode -p $? > ${.TARGET}
