# $FreeBSD: release/10.1.0/sys/modules/hyperv/storvsc/Makefile 255923 2013-09-28 07:04:03Z uqs $

.PATH:  ${.CURDIR}/../../../dev/hyperv/storvsc

KMOD=	hv_storvsc
SRCS =	hv_storvsc_drv_freebsd.c \
	hv_vstorage.h
SRCS+=	bus_if.h device_if.h opt_cam.h

CFLAGS+= -I${.CURDIR}/../../../dev/hyperv/include \
	 -I${.CURDIR}/../../../dev/hyperv/vmbus	\
	 -I${.CURDIR}/../../../dev/hyperv/storvsc

.include <bsd.kmod.mk>
