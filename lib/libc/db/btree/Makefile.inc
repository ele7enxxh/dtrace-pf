#	from @(#)Makefile.inc	8.2 (Berkeley) 7/14/94
# $FreeBSD: release/10.1.0/lib/libc/db/btree/Makefile.inc 107052 2002-11-18 09:50:57Z ru $

.PATH: ${.CURDIR}/db/btree

SRCS+=	bt_close.c bt_conv.c bt_debug.c bt_delete.c bt_get.c bt_open.c \
	bt_overflow.c bt_page.c bt_put.c bt_search.c bt_seq.c bt_split.c \
	bt_utils.c
