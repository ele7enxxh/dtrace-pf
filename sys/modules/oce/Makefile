#
# $FreeBSD: release/10.1.0/sys/modules/oce/Makefile 231512 2012-02-11 08:34:33Z bz $
#

.PATH:	${.CURDIR}/../../dev/oce
KMOD	= oce
SRCS    = oce_if.c oce_hw.c oce_mbox.c oce_util.c oce_queue.c oce_sysctl.c
SRCS    += bus_if.h device_if.h pci_if.h opt_inet.h opt_inet6.h

CFLAGS+= -I${.CURDIR}/../../dev/oce -DSMP

# uncomment for lock profiling statistics
#CFLAGS  += -DLOCK_PROFILING

.include <bsd.kmod.mk>
