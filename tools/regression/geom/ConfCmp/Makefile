# $FreeBSD: release/10.1.0/tools/regression/geom/ConfCmp/Makefile 139103 2004-12-21 08:47:35Z ru $

PROG=	ConfCmp
SRCS+=	ConfCmp.c
SRCS+=	subr_sbuf.c

.PATH:	../../../../sys/kern

NO_OBJ=
WARNS?=	5
CFLAGS+=	 -g -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -static
# Uncomment for ElectricFence
#LDADD += -lefence -L/usr/local/lib

DPADD += ${LIBBSDXML}
LDADD += -lbsdxml

NO_MAN=
CLEANFILES	+= _*

.include <bsd.prog.mk>

test:	${PROG}
	rm -f _* *.core
	./${PROG} a1.conf a1.conf
	./${PROG} a1.conf a1a.conf 
	if ./${PROG} a1.conf a1b.conf > /dev/null 2>&1 ; then exit 1 ; fi
	if ./${PROG} a1.conf a1c.conf > /dev/null 2>&1 ; then exit 1 ; fi
	if ./${PROG} a1.conf a1d.conf > /dev/null 2>&1 ; then exit 1 ; fi
	./${PROG} a2.conf a2.conf
	./${PROG} a2.conf a2a.conf
	if ./${PROG} a2.conf a2b.conf > /dev/null 2>&1 ; then exit 1 ; fi
	if ./${PROG} a2.conf a2c.conf > /dev/null 2>&1 ; then exit 1 ; fi
	if ./${PROG} a2.conf a2d.conf > /dev/null 2>&1 ; then exit 1 ; fi

