#-
# Copyright (c) 2011 Google, Inc.
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
# $FreeBSD: release/10.1.0/lib/atf/Makefile.inc 262849 2014-03-06 13:20:38Z jmmv $

_CFLAGS:=	${CFLAGS}
_CPPFLAGS:=	${CPPFLAGS}
_CXXFLAGS:=	${CXXFLAGS}

CFLAGS+=	-DHAVE_CONFIG_H
CFLAGS+=	-DATF_ARCH='"${MACHINE}"'
CFLAGS+=	-DATF_BUILD_CC='"${CC}"'
CFLAGS+=	-DATF_BUILD_CFLAGS='"${_CFLAGS}"'
CFLAGS+=	-DATF_BUILD_CPP='"${CPP}"'
CFLAGS+=	-DATF_BUILD_CPPFLAGS='"${_CPPFLAGS}"'
CFLAGS+=	-DATF_BUILD_CXX='"${CXX}"'
CFLAGS+=	-DATF_BUILD_CXXFLAGS='"${_CXXFLAGS}"'
CFLAGS+=	-DATF_CONFDIR='"${CONFDIR}/atf"'
CFLAGS+=	-DATF_C_TESTS_BASE='"${TESTSBASE}/lib/atf/libatf-c"'
CFLAGS+=	-DATF_INCLUDEDIR='"${INCLUDEDIR}"'
CFLAGS+=	-DATF_LIBDIR='"${LIBDIR}"'
CFLAGS+=	-DATF_LIBEXECDIR='"${LIBEXECDIR}"'
CFLAGS+=	-DATF_MACHINE='"${MACHINE_ARCH}"'
CFLAGS+=	-DATF_M4='"/usr/bin/m4"'
CFLAGS+=	-DATF_PKGDATADIR='"${SHAREDIR}/atf"'
CFLAGS+=	-DATF_SHELL='"/bin/sh"'
CFLAGS+=	-DATF_WORKDIR='"/tmp"'

WARNS?=		3

# vim: syntax=make
