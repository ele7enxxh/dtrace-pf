#/*
# * Copyright (c) 2013-2014 Qlogic Corporation
# * All rights reserved.
# *
# *  Redistribution and use in source and binary forms, with or without
# *  modification, are permitted provided that the following conditions
# *  are met:
# *
# *  1. Redistributions of source code must retain the above copyright
# *     notice, this list of conditions and the following disclaimer.
# *  2. Redistributions in binary form must reproduce the above copyright
# *     notice, this list of conditions and the following disclaimer in the
# *     documentation and/or other materials provided with the distribution.
# *
# *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# *  POSSIBILITY OF SUCH DAMAGE.
# */
#/*
# * File : Makefile
# * Author : David C Somayajulu, Qlogic Corporation, Aliso Viejo, CA 92656.
# */
#
# $FreeBSD: release/10.1.0/sys/modules/qlxge/Makefile 252206 2013-06-25 17:50:22Z davidcs $
#

.PATH: ${.CURDIR}/../../dev/qlxge

KMOD=if_qlxge
SRCS=qls_os.c qls_dbg.c qls_hw.c qls_isr.c qls_dump.c
SRCS+=qls_ioctl.c
SRCS+= device_if.h bus_if.h pci_if.h

CFLAGS += -DQL_DBG

clean:
	rm -f opt_bdg.h device_if.h bus_if.h pci_if.h export_syms
	rm -f *.o *.kld *.ko
	rm -f @ machine x86

.include <bsd.kmod.mk>

