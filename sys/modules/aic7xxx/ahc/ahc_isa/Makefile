
# $FreeBSD: release/10.1.0/sys/modules/aic7xxx/ahc/ahc_isa/Makefile 270284 2014-08-21 17:18:21Z ian $

.PATH:	${.CURDIR}/../../../../dev/aic7xxx
KMOD=		ahc_isa

SRCS=	ahc_isa.c
SRCS+=	device_if.h bus_if.h pci_if.h isa_if.h
SRCS+=	opt_scsi.h opt_cam.h opt_aic7xxx.h

CFLAGS+=	-I${.CURDIR}/../../../../dev/aic7xxx -I..

.include <bsd.kmod.mk>
