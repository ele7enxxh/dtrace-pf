#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/rmdir/13.t 210984 2010-08-06 23:58:54Z pjd $

desc="rmdir returns EBUSY if the directory to be removed is the mount point for a mounted file system"

dir=`dirname $0`
. ${dir}/../misc.sh

[ "${os}" = "FreeBSD" ] || quick_exit

echo "1..3"

n0=`namegen`

expect 0 mkdir ${n0} 0755
n=`mdconfig -a -n -t malloc -s 1m`
newfs /dev/md${n} >/dev/null
mount /dev/md${n} ${n0}
expect EBUSY rmdir ${n0}
umount /dev/md${n}
mdconfig -d -u ${n}
expect 0 rmdir ${n0}
