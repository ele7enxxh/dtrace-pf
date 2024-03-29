# $FreeBSD: release/10.1.0/lib/libkse/support/Makefile.inc 209118 2010-06-13 01:27:29Z imp $

.PATH: ${.CURDIR}/support ${.CURDIR}/../libc/gen ${.CURDIR}/../libc/string
.PATH: ${.CURDIR}/../libc/${MACHINE_CPUARCH}/sys

CFLAGS+= -I${.CURDIR}/../libc/${MACHINE_CPUARCH}

SYSCALLS= clock_gettime \
	kse_create \
	kse_exit \
	kse_release \
	kse_switchin \
	kse_thr_interrupt \
	kse_wakeup \
	sigaction \
	sigprocmask \
	sigtimedwait \
	write

SYSCALL_SRC=   ${SYSCALLS:S/$/.S/}
SYSCALL_OBJ=   ${SYSCALLS:S/$/.So/}

${SYSCALL_SRC}:
	printf '#include "SYS.h"\nRSYSCALL(${.PREFIX})\n' > ${.TARGET}

LIBC_OBJS= sigsetops.So \
	bcopy.So \
	bzero.So \
	cerror.So \
	memcpy.So \
	memset.So \
	strcpy.So \
	strlen.So

SOBJS+=	thr_libc.So
CLEANFILES+= ${SYSCALL_SRC} ${SYSCALL_OBJ} ${LIBC_OBJS}

thr_libc.So: ${SYSCALL_OBJ} ${LIBC_OBJS}
	${CC} -fPIC -nostdlib -o ${.TARGET} -r ${.ALLSRC}

