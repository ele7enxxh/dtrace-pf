#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/unlink/02.t 211178 2010-08-11 16:33:17Z pjd $

desc="unlink returns ENAMETOOLONG if a component of a pathname exceeded {NAME_MAX} characters"

dir=`dirname $0`
. ${dir}/../misc.sh

echo "1..4"

nx=`namegen_max`
nxx="${nx}x"

expect 0 create ${nx} 0644
expect 0 unlink ${nx}
expect ENOENT unlink ${nx}
expect ENAMETOOLONG unlink ${nxx}
