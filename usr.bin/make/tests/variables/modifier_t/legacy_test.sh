#! /bin/sh
# $FreeBSD: release/10.1.0/usr.bin/make/tests/variables/modifier_t/legacy_test.sh 263346 2014-03-19 12:29:20Z jmmv $

. $(dirname $0)/../../common.sh

# Description
DESC="Variable expansion with t modifiers"

# Run
TEST_N=3

eval_cmd $*
