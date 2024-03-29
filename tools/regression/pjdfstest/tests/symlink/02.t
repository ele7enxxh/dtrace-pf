#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/symlink/02.t 211178 2010-08-11 16:33:17Z pjd $

desc="symlink returns ENAMETOOLONG if a component of the name2 pathname exceeded {NAME_MAX} characters"

dir=`dirname $0`
. ${dir}/../misc.sh

echo "1..7"

n0=`namegen`
nx=`namegen_max`
nxx="${nx}x"

expect 0 symlink ${nx} ${n0}
expect 0 unlink ${n0}
expect 0 symlink ${n0} ${nx}
expect 0 unlink ${nx}

expect ENAMETOOLONG symlink ${n0} ${nxx}
expect 0 symlink ${nxx} ${n0}
expect 0 unlink ${n0}
