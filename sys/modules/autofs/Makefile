# $FreeBSD: release/10.1.0/sys/modules/autofs/Makefile 270096 2014-08-17 09:44:42Z trasz $

.PATH: ${.CURDIR}/../../fs/autofs

KMOD=	autofs
SRCS=	vnode_if.h \
	autofs.c \
	autofs_vnops.c \
	autofs_vfsops.c

.include <bsd.kmod.mk>
