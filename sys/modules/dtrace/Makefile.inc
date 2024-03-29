# $FreeBSD: release/10.1.0/sys/modules/dtrace/Makefile.inc 211690 2010-08-23 06:13:29Z imp $

IGNORE_PRAGMA=	1

load	:
	-kldload cyclic
	-kldload dtrace
.if ${MACHINE_CPUARCH} == "i386"
	-kldload sdt
	-kldload lockstat
	-kldload fbt
	-kldload prototype
.endif
	-kldload profile
	-kldload systrace
	kldstat

unload	:
	-kldunload systrace
	-kldunload profile
.if ${MACHINE_CPUARCH} == "i386"
	-kldunload prototype
	-kldunload fbt
	-kldunload lockstat
	-kldunload sdt
.endif
	-kldunload dtrace
	-kldunload cyclic
	kldstat
