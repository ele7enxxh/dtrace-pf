#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/geom_nop/test-1.t 153186 2005-12-07 01:27:23Z pjd $

. `dirname $0`/conf.sh

echo "1..1"

us=45

mdconfig -a -t malloc -s 1M -u $us || exit 1

gnop create /dev/md${us} || exit 1

# Size of created device should be 1MB.

size=`diskinfo /dev/md${us}.nop | awk '{print $3}'`

if [ $size -eq 1048576 ]; then
	echo "ok 1"
else
	echo "not ok 1"
fi

gnop destroy md${us}.nop
mdconfig -d -u $us
