/*
 * Copyright (c) 2010 The FreeBSD Foundation 
 * All rights reserved. 
 * 
 * This software was developed by Rui Paulo under sponsorship from the
 * FreeBSD Foundation. 
 *  
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */ 

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/lib/libproc/proc_rtld.c 270731 2014-08-27 19:51:42Z markj $");

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rtld_db.h>
#include "libproc.h"
#include "_libproc.h"

static int
map_iter(const rd_loadobj_t *lop, void *arg)
{
	struct proc_handle *phdl = arg;

	if (phdl->nobjs >= phdl->rdobjsz) {
		phdl->rdobjsz *= 2;
		phdl->rdobjs = reallocf(phdl->rdobjs, sizeof(*phdl->rdobjs) *
		    phdl->rdobjsz);
		if (phdl->rdobjs == NULL)
			return (-1);
	}
	if (strcmp(lop->rdl_path, phdl->execname) == 0 &&
	    (lop->rdl_prot & RD_RDL_X) != 0)
		phdl->rdexec = &phdl->rdobjs[phdl->nobjs];
	memcpy(&phdl->rdobjs[phdl->nobjs++], lop, sizeof(*lop));

	return (0);
}

rd_agent_t *
proc_rdagent(struct proc_handle *phdl)
{
	if (phdl->rdap == NULL && phdl->status != PS_UNDEAD &&
	    phdl->status != PS_IDLE) {
		if ((phdl->rdap = rd_new(phdl)) != NULL) {
			phdl->rdobjs = malloc(sizeof(*phdl->rdobjs) * 64);
			phdl->rdobjsz = 64;
			if (phdl->rdobjs == NULL)
				return (phdl->rdap);
			rd_loadobj_iter(phdl->rdap, map_iter, phdl);
		}
	}

	return (phdl->rdap);
}

void
proc_updatesyms(struct proc_handle *phdl)
{

	memset(phdl->rdobjs, 0, sizeof(*phdl->rdobjs) * phdl->rdobjsz);
	phdl->nobjs = 0;
	rd_loadobj_iter(phdl->rdap, map_iter, phdl);
}
