#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/rename/16.t 210984 2010-08-06 23:58:54Z pjd $

desc="rename returns EROFS if the requested link requires writing in a directory on a read-only file system"

dir=`dirname $0`
. ${dir}/../misc.sh

[ "${os}" = "FreeBSD" ] || quick_exit

echo "1..8"

n0=`namegen`
n1=`namegen`
n2=`namegen`

expect 0 mkdir ${n0} 0755
n=`mdconfig -a -n -t malloc -s 1m`
newfs /dev/md${n} >/dev/null
mount /dev/md${n} ${n0}
expect 0 create ${n0}/${n1} 0644
mount -ur /dev/md${n}

expect EROFS rename ${n0}/${n1} ${n0}/${n2}
expect EROFS rename ${n0}/${n1} ${n2}
expect 0 create ${n2} 0644
expect EROFS rename ${n2} ${n0}/${n2}
expect 0 unlink ${n2}

umount /dev/md${n}
mdconfig -d -u ${n}
expect 0 rmdir ${n0}
