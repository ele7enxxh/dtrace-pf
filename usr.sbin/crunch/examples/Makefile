# $FreeBSD: release/10.1.0/usr.sbin/crunch/examples/Makefile 241298 2012-10-06 20:01:05Z marcel $

CRUNCHED=	fixit

# below is boiler-plate to make $(CRUNCHED) from $(CRUNCHED).conf
# I'd use PROG instead of CRUNCHED, but the system makefiles REALLY want
# to build things in the normal way if you use PROG.

CONF=	$(CRUNCHED).conf

OUTMK=	$(CRUNCHED).mk
OUTPUTS= $(OUTMK) $(CRUNCHED).c $(CRUNCHED).cache

NO_MAN=
CLEANFILES+= $(CRUNCHED) *.o *.lo *.c *.mk *.cache
CLEANDIRFILES+= $(OUTPUTS)

all: $(CRUNCHED)
exe: $(CRUNCHED)

$(OUTPUTS): $(CONF)
	MAKE=${MAKE} crunchgen ${.CURDIR}/$(CONF)

$(CRUNCHED): $(OUTPUTS) submake

submake:
	${MAKE} -f $(OUTMK)
objs:
	${MAKE} -f $(OUTMK) objs
cleandir:
	rm -f $(CLEANDIRFILES)

.include <bsd.prog.mk>
