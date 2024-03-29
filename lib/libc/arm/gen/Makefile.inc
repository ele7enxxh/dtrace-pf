#	@(#)Makefile.inc	8.1 (Berkeley) 6/4/93
# $FreeBSD: release/10.1.0/lib/libc/arm/gen/Makefile.inc 266370 2014-05-17 22:19:16Z ian $

SRCS+=	_ctx_start.S _setjmp.S _set_tp.c alloca.S fabs.c \
	getcontextx.c infinity.c ldexp.c makecontext.c \
	__aeabi_read_tp.S setjmp.S signalcontext.c sigsetjmp.S flt_rounds.c \
	arm_initfini.c

.if ${MK_ARM_EABI} == "no"
SRCS+=	divsi3.S
.endif

.if ${MACHINE_ARCH} == "armv6hf"
SRCS+=	fpgetmask_vfp.c fpgetround_vfp.c fpgetsticky_vfp.c fpsetmask_vfp.c \
	fpsetround_vfp.c fpsetsticky_vfp.c
.endif
