#!/bin/sh -
# Copyright (c) 2013 Garrett Cooper
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: release/10.1.0/include/mk-osreldate.sh 255957 2013-09-30 21:01:04Z ian $

set -e

CURDIR=$(pwd)
ECHO=${ECHO:=echo}

tmpfile=$(mktemp osreldate.XXXXXXXX)
trap "rm -f $tmpfile" EXIT

${ECHO} creating osreldate.h from newvers.sh

export PARAMFILE="${PARAM_H:=$CURDIR/../sys/sys/param.h}"
set +e
. "${NEWVERS_SH:=$CURDIR/../sys/conf/newvers.sh}" || exit 1
set -e
cat > $tmpfile <<EOF
$COPYRIGHT
#ifdef _KERNEL
#error "<osreldate.h> cannot be used in the kernel, use <sys/param.h>"
#else
#undef __FreeBSD_version
#define __FreeBSD_version $RELDATE
#endif
EOF
chmod 644 $tmpfile
mv $tmpfile osreldate.h
