# $FreeBSD: release/10.1.0/lib/libc/resolv/Makefile.inc 270838 2014-08-30 10:16:25Z ume $

# resolv sources
.PATH: ${.CURDIR}/resolv

SRCS+=	herror.c h_errno.c mtctxres.c res_comp.c res_data.c res_debug.c \
	res_findzonecut.c res_init.c res_mkquery.c res_mkupdate.c \
	res_query.c res_send.c res_state.c res_update.c

SYM_MAPS+= ${.CURDIR}/resolv/Symbol.map

CFLAGS+=-I${.CURDIR}/../libmd
