# $FreeBSD: release/10.1.0/sys/boot/ia64/common/Makefile 242688 2012-11-07 02:14:03Z dteske $

.include <bsd.own.mk>
MK_SSP=		no

LIB=		ia64
INTERNALLIB=

SRCS=		autoload.c bootinfo.c copy.c devicename.c exec.c icache.c

CFLAGS+=	-I${.CURDIR}/../../efi/include
CFLAGS+=	-I${.CURDIR}/../../efi/include/${MACHINE_CPUARCH}
CFLAGS+=	-I${.CURDIR}/../../..
CFLAGS+=	-I${.CURDIR}/../../../../lib/libstand

.if ${MK_FORTH} != "no"
BOOT_FORTH=	yes
CFLAGS+=	-DBOOT_FORTH
CFLAGS+=	-I${.CURDIR}/../../ficl
CFLAGS+=	-I${.CURDIR}/../../ficl/${MACHINE_CPUARCH}
.endif

.PATH: ${.CURDIR}/../../common
.include "${.CURDIR}/../../common/Makefile.inc"

CFLAGS+=	-I${.CURDIR}/../../common

FILES+=		loader.help
CLEANFILES+=	loader.help
loader.help: help.common
	cat ${.ALLSRC} | awk -f ${.CURDIR}/../../common/merge_help.awk \
	    > ${.TARGET}

.PATH: ${.CURDIR}/../../forth
FILES+=		loader.4th support.4th loader.conf
FILES+=		screen.4th frames.4th
FILES+=		beastie.4th brand.4th check-password.4th color.4th delay.4th
FILES+=		menu.4th menu-commands.4th menusets.4th shortcuts.4th version.4th
.if !exists(${DESTDIR}/boot/loader.rc)
FILES+=		loader.rc
.endif
.if !exists(${DESTDIR}/boot/menu.rc)
FILES+=		menu.rc
.endif
FILESDIR_loader.conf= /boot/defaults

.include <bsd.lib.mk>
