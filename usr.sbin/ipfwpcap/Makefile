#
# From: Id: Makefile,v 1.2 2004/01/15 16:20:56 pkern Exp
#
# $FreeBSD: release/10.1.0/usr.sbin/ipfwpcap/Makefile 201390 2010-01-02 11:07:44Z ed $
#

PROG=	ipfwpcap

LDADD=	-lpcap
DPADD=	${LIBPCAP}

MAN=	ipfwpcap.8

.include <bsd.prog.mk>

test:	$(CMD)
	-rm /var/run/ipfwpcap.2000.pid
	./ipfwpcap -d 2000 - | tcpdump -r - -n -s 2000 -X

