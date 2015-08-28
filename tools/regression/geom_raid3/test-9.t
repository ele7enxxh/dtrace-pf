#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/geom_raid3/test-9.t 153187 2005-12-07 01:28:59Z pjd $

. `dirname $0`/conf.sh

echo "1..1"

us0=45
us1=`expr $us0 + 1`
us2=`expr $us0 + 2`
ddbs=2048
nblocks1=1024
nblocks2=`expr $nblocks1 / \( $ddbs / 512 \)`
src=`mktemp /tmp/$base.XXXXXX` || exit 1
dst=`mktemp /tmp/$base.XXXXXX` || exit 1

dd if=/dev/random of=${src} bs=$ddbs count=$nblocks2 >/dev/null 2>&1

mdconfig -a -t malloc -s `expr $nblocks1 + 1` -u $us0 || exit 1
mdconfig -a -t malloc -s `expr $nblocks1 + 1` -u $us1 || exit 1
mdconfig -a -t malloc -s `expr $nblocks1 + 1` -u $us2 || exit 1

graid3 label $name /dev/md${us0} /dev/md${us1} /dev/md${us2} || exit 1
devwait

#
# Writing without PARITY component and rebuild of PARITY component.
#
graid3 remove -n 2 $name
dd if=/dev/zero of=/dev/md${us2} bs=512 count=`expr $nblocks1 + 1` >/dev/null 2>&1
dd if=${src} of=/dev/raid3/${name} bs=$ddbs count=$nblocks2 >/dev/null 2>&1
graid3 insert -n 2 $name md${us2}
sleep 1
# Remove DATA component, so PARITY component can be used while reading.
graid3 remove -n 1 $name
dd if=/dev/zero of=/dev/md${us1} bs=512 count=`expr $nblocks1 + 1` >/dev/null 2>&1

dd if=/dev/raid3/${name} of=${dst} bs=$ddbs count=$nblocks2 >/dev/null 2>&1
if [ `md5 -q ${src}` != `md5 -q ${dst}` ]; then
	echo "not ok 1"
else
	echo "ok 1"
fi

graid3 stop $name
mdconfig -d -u $us0
mdconfig -d -u $us1
mdconfig -d -u $us2
rm -f ${src} ${dst}
