#       from @(#)Makefile.inc	8.1 (Berkeley) 6/4/93
# $FreeBSD: release/10.1.0/lib/libc/db/recno/Makefile.inc 107052 2002-11-18 09:50:57Z ru $

.PATH: ${.CURDIR}/db/recno

SRCS+=	rec_close.c rec_delete.c rec_get.c rec_open.c rec_put.c rec_search.c \
	rec_seq.c rec_utils.c
