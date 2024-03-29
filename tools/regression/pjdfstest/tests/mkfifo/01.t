#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/mkfifo/01.t 211474 2010-08-18 22:06:43Z pjd $

desc="mkfifo returns ENOTDIR if a component of the path prefix is not a directory"

dir=`dirname $0`
. ${dir}/../misc.sh

echo "1..17"

n0=`namegen`
n1=`namegen`

expect 0 mkdir ${n0} 0755
for type in regular fifo block char socket; do
	create_file ${type} ${n0}/${n1}
	expect ENOTDIR mkfifo ${n0}/${n1}/test 0644
	expect 0 unlink ${n0}/${n1}
done
expect 0 rmdir ${n0}
