# $FreeBSD: release/10.1.0/libexec/rpc.rusersd/Makefile 241777 2012-10-20 10:33:15Z ed $

PROG =	rpc.rusersd
SRCS =	rusersd.c rusers_proc.c extern.h
MAN =	rpc.rusersd.8

DPADD=	${LIBRPCSVC} ${LIBUTIL}
LDADD=	-lrpcsvc -lutil

#.if exists(/usr/X11R6/include/X11/extensions/xidle.h)
#CFLAGS+= -DXIDLE
#LDADD+= -L/usr/X11R6/lib -lXext -lX11
#.endif

.include <bsd.prog.mk>
