#	from: @(#)Makefile    5.10 (Berkeley) 6/24/90
# $FreeBSD: release/10.1.0/lib/librpcsvc/Makefile 231118 2012-02-07 09:27:07Z dim $

.include <bsd.own.mk>

.PATH: ${.CURDIR}/../../include/rpcsvc

LIB=    rpcsvc

RPCSRCS= klm_prot.x mount.x nfs_prot.x nlm_prot.x rex.x rnusers.x \
	rquota.x rstat.x rwall.x sm_inter.x spray.x yppasswd.x ypxfrd.x \
	ypupdate_prot.x

OTHERSRCS=  rnusers.c rstat.c rwall.c
SECRPCSRCS= secretkey.c xcrypt.c

.if ${MK_NIS} != "no"
OTHERSRCS+= yp_passwd.c yp_update.c
.endif

RPCCOM=	RPCGEN_CPP=${CPP:Q} rpcgen -C

INCDIRS= -I${DESTDIR}/usr/include/rpcsvc

CFLAGS+= -DYP ${INCDIRS}

GENSRCS= ${RPCSRCS:R:S/$/_xdr.c/g}
SRCS+= ${GENSRCS} ${OTHERSRCS} ${SECRPCSRCS}

CLEANFILES+= ${GENSRCS}

WARNS?=	1

.include <bsd.lib.mk>

.SUFFIXES: .x _xdr.c

.x_xdr.c:
	${RPCCOM} -c ${.IMPSRC} -o ${.TARGET}

OBJS=	${RPCSRCS:R:S/$/_xdr.o/g} ${SECRPCSRCS:R:S/$/.o/g} \
	${OTHERSRCS:R:S/$/.o/g}
