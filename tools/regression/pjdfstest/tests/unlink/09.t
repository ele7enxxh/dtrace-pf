#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/pjdfstest/tests/unlink/09.t 210984 2010-08-06 23:58:54Z pjd $

desc="unlink returns EPERM if the named file has its immutable, undeletable or append-only flag set"

dir=`dirname $0`
. ${dir}/../misc.sh

require chflags

case "${os}:${fs}" in
FreeBSD:ZFS)
	echo "1..15"
	;;
FreeBSD:UFS)
	echo "1..30"
	;;
*)
	quick_exit
esac

n0=`namegen`

expect 0 create ${n0} 0644
expect 0 chflags ${n0} SF_IMMUTABLE
expect EPERM unlink ${n0}
expect 0 chflags ${n0} none
expect 0 unlink ${n0}

expect 0 create ${n0} 0644
expect 0 chflags ${n0} SF_NOUNLINK
expect EPERM unlink ${n0}
expect 0 chflags ${n0} none
expect 0 unlink ${n0}

expect 0 create ${n0} 0644
expect 0 chflags ${n0} SF_APPEND
todo FreeBSD:ZFS "Removing a file protected by SF_APPEND should return EPERM."
expect EPERM unlink ${n0}
todo FreeBSD:ZFS "Removing a file protected by SF_APPEND should return EPERM."
expect 0 chflags ${n0} none
todo FreeBSD:ZFS "Removing a file protected by SF_APPEND should return EPERM."
expect 0 unlink ${n0}

case "${os}:${fs}" in
FreeBSD:UFS)
	expect 0 create ${n0} 0644
	expect 0 chflags ${n0} UF_IMMUTABLE
	expect EPERM unlink ${n0}
	expect 0 chflags ${n0} none
	expect 0 unlink ${n0}

	expect 0 create ${n0} 0644
	expect 0 chflags ${n0} UF_NOUNLINK
	expect EPERM unlink ${n0}
	expect 0 chflags ${n0} none
	expect 0 unlink ${n0}

	expect 0 create ${n0} 0644
	expect 0 chflags ${n0} UF_APPEND
	expect EPERM unlink ${n0}
	expect 0 chflags ${n0} none
	expect 0 unlink ${n0}
	;;
esac
