#	From: @(#)Makefile	8.1 (Berkeley) 6/10/93
# $FreeBSD: release/10.1.0/share/doc/smm/Makefile 239739 2012-08-27 17:15:14Z jhb $

.include <bsd.own.mk>

# The following modules do not describe FreeBSD:
# 14.uucpimpl, 15.uucpnet

# The following modules do not build/install:
# 13.amd (documentation is TeXinfo)
# 16.security 17.password (encumbered)

SUBDIR=	title \
	contents \
	01.setup \
	02.config \
	03.fsck \
	04.quotas \
	05.fastfs \
	06.nfs \
	${_07.lpd} \
	${_08.sendmailop} \
	11.timedop \
	12.timed \
	18.net

.if ${MK_SENDMAIL} != "no"
_08.sendmailop=	08.sendmailop 
.endif

.if ${MK_LPR} != "no"
_07.lpd=	07.lpd
.endif

.include <bsd.subdir.mk>
