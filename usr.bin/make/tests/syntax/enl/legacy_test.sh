#! /bin/sh
# $FreeBSD: release/10.1.0/usr.bin/make/tests/syntax/enl/legacy_test.sh 263346 2014-03-19 12:29:20Z jmmv $

. $(dirname $0)/../../common.sh

# Description
DESC="Test escaped new-lines handling."

# Run
TEST_N=5
TEST_2_TODO="bug in parser"

eval_cmd $*
