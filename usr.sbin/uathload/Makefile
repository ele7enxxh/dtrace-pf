# $FreeBSD: release/10.1.0/usr.sbin/uathload/Makefile 218749 2011-02-16 21:04:47Z dim $

PROG=	uathload
MAN=	uathload.8

SRCS=	uathload.c ar5523.bin

CLEANFILES=	ar5523.bin

.if ${MACHINE_ARCH} == "i386" || ${MACHINE_ARCH} == "amd64" || ${MACHINE_ARCH} == "powerpc" || ${MACHINE_ARCH} == "powerpc64"
LDFLAGS+=	-Wl,-z,noexecstack
.endif

ar5523.bin: ${.CURDIR}/../../sys/contrib/dev/uath/ar5523.bin.uu
	uudecode -p ${.CURDIR}/../../sys/contrib/dev/uath/ar5523.bin.uu > ${.TARGET}

ar5523.o: ar5523.bin
	${LD} -b binary -d -warn-common -r -d -o ${.TARGET} ar5523.bin

.include <bsd.prog.mk>
