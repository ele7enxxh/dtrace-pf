# $FreeBSD: release/10.1.0/sys/modules/acpi/acpi_fujitsu/Makefile 193530 2009-06-05 18:44:36Z jkim $

.PATH:	${.CURDIR}/../../../dev/acpi_support

KMOD=	acpi_fujitsu
CFLAGS+=-I${.CURDIR}/../../../dev/acpi_support
SRCS=	acpi_fujitsu.c opt_acpi.h acpi_if.h device_if.h bus_if.h

.include <bsd.kmod.mk>
