# $FreeBSD: release/10.1.0/sys/modules/ctl/Makefile 269296 2014-07-30 07:18:32Z mav $

.PATH: ${.CURDIR}/../../cam/ctl

KMOD=	ctl

SRCS=	ctl.c
SRCS+=	ctl_backend.c
SRCS+=	ctl_backend_block.c
SRCS+=	ctl_backend_ramdisk.c
SRCS+=	ctl_cmd_table.c
SRCS+=	ctl_frontend.c
SRCS+=	ctl_frontend_cam_sim.c
SRCS+=	ctl_frontend_internal.c
SRCS+=	ctl_frontend_iscsi.c
SRCS+=	ctl_scsi_all.c
SRCS+=	ctl_tpc.c
SRCS+=	ctl_tpc_local.c
SRCS+=	ctl_error.c
SRCS+=	ctl_util.c
SRCS+=	scsi_ctl.c
SRCS+=	bus_if.h
SRCS+=	device_if.h
SRCS+=	vnode_if.h
SRCS+=	opt_cam.h
SRCS+=	opt_kdtrace.h

#CFLAGS+=-DICL_KERNEL_PROXY

.include <bsd.kmod.mk>
