# $FreeBSD: release/10.1.0/sys/modules/hyperv/utilities/Makefile 272322 2014-09-30 17:54:57Z delphij $

.PATH:	${.CURDIR}/../../../dev/hyperv/utilities

KMOD=	hv_utils
SRCS=	hv_util.c hv_kvp.c
SRCS+=	bus_if.h device_if.h

CFLAGS+= -I${.CURDIR}/../../../dev/hyperv/include \
	 -I${.CURDIR}/../../../dev/hyperv/vmbus

.include <bsd.kmod.mk>
