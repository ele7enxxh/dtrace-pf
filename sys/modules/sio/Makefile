# $FreeBSD: release/10.1.0/sys/modules/sio/Makefile 225214 2011-08-27 14:24:27Z rwatson $

.if ${MACHINE} == "pc98"
.PATH: ${.CURDIR}/../../pc98/cbus
.endif
.PATH: ${.CURDIR}/../../dev/sio

KMOD=	sio
SRCS=	bus_if.h card_if.h device_if.h isa_if.h pci_if.h serdev_if.h \
	opt_compat.h opt_gdb.h opt_kdb.h opt_sio.h \
	sio.c sio_pccard.c sio_pci.c sio_puc.c pccarddevs.h
.if ${MACHINE} == "pc98"
SRCS+=	sio_cbus.c
.else
SRCS+=	sio_isa.c
.endif

.if !defined(KERNBUILDDIR)
opt_compat.h:
	echo "#define COMPAT_43 1" > ${.TARGET}

.if ${MACHINE} == "pc98"
opt_sio.h:
	echo "#define COM_MULTIPORT 1" > ${.TARGET}
	echo "#define COM_ESP 1" >> ${.TARGET}
.endif
.endif

.include <bsd.kmod.mk>
