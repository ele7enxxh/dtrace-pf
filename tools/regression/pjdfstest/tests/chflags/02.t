#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/chflags/02.t 211178 2010-08-11 16:33:17Z pjd $

desc="chflags returns ENAMETOOLONG if a component of a pathname exceeded {NAME_MAX} characters"

dir=`dirname $0`
. ${dir}/../misc.sh

require chflags

echo "1..12"

nx=`namegen_max`
nxx="${nx}x"

expect 0 create ${nx} 0644
expect 0 chflags ${nx} SF_IMMUTABLE
expect SF_IMMUTABLE stat ${nx} flags
expect 0 chflags ${nx} none
expect 0 unlink ${nx}
expect ENAMETOOLONG chflags ${nxx} SF_IMMUTABLE

expect 0 create ${nx} 0644
expect 0 lchflags ${nx} SF_IMMUTABLE
expect SF_IMMUTABLE stat ${nx} flags
expect 0 lchflags ${nx} none
expect 0 unlink ${nx}
expect ENAMETOOLONG lchflags ${nxx} SF_IMMUTABLE
