#!/bin/sh
# $FreeBSD: release/10.1.0/tools/regression/lib/libc/locale/test-mbrlen.t 137587 2004-11-11 19:47:55Z nik $

cd `dirname $0`

executable=`basename $0 .t`

make $executable 2>&1 > /dev/null

exec ./$executable
