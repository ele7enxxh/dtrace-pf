#	$OpenBSD: hash.m4,v 1.3 2003/06/03 02:56:11 millert Exp $
#	$NetBSD: hash.m4,v 1.4 1995/09/28 05:37:58 tls Exp $
#
# Copyright (c) 1989, 1993
#	The Regents of the University of California.  All rights reserved.
#
# This code is derived from software contributed to Berkeley by
# Ozan Yigit.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: release/10.1.0/usr.bin/m4/TEST/hash.m4 250226 2013-05-03 23:29:38Z jkim $
#
#	@(#)hash.m4	8.1 (Berkeley) 6/6/93
#

dnl	This probably will not run on any m4 that cannot
dnl	handle char constants in eval.
dnl
changequote(<,>) define(HASHVAL,99) dnl
define(hash,<eval(str(substr($1,1),0)%HASHVAL)>) dnl
define(str,
	<ifelse($1,",$2,
		<str(substr(<$1>,1),<eval($2+'substr($1,0,1)')>)>)
	>) dnl
define(KEYWORD,<$1,hash($1),>) dnl
define(TSTART,
<struct prehash {
	char *keyword;
	int   hashval;
} keytab[] = {>) dnl
define(TEND,<	"",0
};>) dnl
