# $FreeBSD: release/10.1.0/lib/libc/sparc64/Makefile.inc 246117 2013-01-30 12:48:16Z kib $
#
# Machine dependent definitions for the ultra sparc architecture.
#

.include "fpu/Makefile.inc"

SRCS+=	__vdso_gettc.c

# Long double is quad precision
GDTOASRCS+=strtorQ.c
MDSRCS+=machdep_ldisQ.c
SYM_MAPS+=${.CURDIR}/sparc64/Symbol.map
