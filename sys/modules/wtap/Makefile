# $FreeBSD: release/10.1.0/sys/modules/wtap/Makefile 230273 2012-01-17 18:20:34Z adrian $


# Declare Name of kernel module
KMOD	=  wtap

# Enumerate Source files for kernel module
.PATH:	${.CURDIR}/../../dev/wtap
SRCS	=  if_wtap_module.c if_wtap.c if_medium.c

.PATH:	${.CURDIR}/../../dev/wtap/wtap_hal
SRCS	+= hal.c

.PATH:	${.CURDIR}/../../dev/wtap/plugins
SRCS	+= visibility.c

SRCS	+= opt_global.h

.if defined(KERNBUILDDIR)
MKDEP=          -include ${KERNBUILDDIR}/opt_global.h
.else
CFLAGS+=        -include opt_global.h
MKDEP=          -include opt_global.h

opt_global.h:
	echo "#define VIMAGE 1" > ${.TARGET}
.endif

# Include kernel module makefile
.include <bsd.kmod.mk>
