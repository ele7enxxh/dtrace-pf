#	@(#)Makefile.inc	8.1 (Berkeley) 6/2/93
# $FreeBSD: release/10.1.0/lib/libc/compat-43/Makefile.inc 211774 2010-08-24 20:54:43Z imp $

# compat-43 sources
.PATH: ${.CURDIR}/${LIBC_ARCH}/compat-43 ${.CURDIR}/compat-43

SRCS+=	creat.c gethostid.c getwd.c killpg.c sethostid.c setpgrp.c \
	setrgid.c setruid.c sigcompat.c

SYM_MAPS+=${.CURDIR}/compat-43/Symbol.map

MAN+=	creat.2 killpg.2 sigpause.2 sigsetmask.2 sigvec.2
MAN+=	gethostid.3 setruid.3

MLINKS+=gethostid.3 sethostid.3
MLINKS+=sigpause.2 sighold.2
MLINKS+=sigpause.2 sigignore.2
MLINKS+=sigpause.2 sigrelse.2
MLINKS+=sigpause.2 sigset.2
MLINKS+=sigpause.2 xsi_sigpause.2
MLINKS+=setruid.3 setrgid.3

MLINKS+=sigsetmask.2 sigblock.2
MLINKS+=sigsetmask.2 sigmask.2
