/*	$NetBSD: cpuswitch.S,v 1.41 2003/11/15 08:44:18 scw Exp $	*/

/*-
 * Copyright 2003 Wasabi Systems, Inc.
 * All rights reserved.
 *
 * Written by Steve C. Woodford for Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed for the NetBSD Project by
 *      Wasabi Systems, Inc.
 * 4. The name of Wasabi Systems, Inc. may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY WASABI SYSTEMS, INC. ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL WASABI SYSTEMS, INC
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*-
 * Copyright (c) 1994-1998 Mark Brinicombe.
 * Copyright (c) 1994 Brini.
 * All rights reserved.
 *
 * This code is derived from software written for Brini by Mark Brinicombe
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Brini.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BRINI ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BRINI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * RiscBSD kernel project
 *
 * cpuswitch.S
 *
 * cpu switching functions
 *
 * Created      : 15/10/94
 *
 */

#include "assym.s"
#include "opt_sched.h"

#include <machine/asm.h>
#include <machine/asmacros.h>
#include <machine/armreg.h>
#include <machine/vfp.h>

__FBSDID("$FreeBSD: release/10.1.0/sys/arm/arm/swtch.S 266311 2014-05-17 13:53:38Z ian $");

#define DOMAIN_CLIENT	0x01

#if defined(_ARM_ARCH_6) && defined(SMP)
#define GET_PCPU(tmp, tmp2) \
	mrc 	p15, 0, tmp, c0, c0, 5;	\
	and	tmp, tmp, #0xf;		\
	ldr 	tmp2, .Lcurpcpu+4;	\
	mul 	tmp, tmp, tmp2;		\
	ldr	tmp2, .Lcurpcpu;	\
	add	tmp, tmp, tmp2;
#else

#define GET_PCPU(tmp, tmp2) \
	ldr	tmp, .Lcurpcpu
#endif

#ifdef VFP
	.fpu vfp	/* allow VFP instructions */
#endif

.Lcurpcpu:
        .word   _C_LABEL(__pcpu)
	.word	PCPU_SIZE
.Lcpufuncs:	
	.word	_C_LABEL(cpufuncs)
.Lblocked_lock:
	.word	_C_LABEL(blocked_lock)

ENTRY(cpu_throw)
	mov	r5, r1

	/*
	 * r0 = oldtd
	 * r5 = newtd
	 */

#ifdef VFP				/* This thread is dying, disable */
	bl	_C_LABEL(vfp_discard)	/* VFP without preserving state. */
#endif

	GET_PCPU(r7, r9)
	ldr	r7, [r5, #(TD_PCB)]		/* r7 = new thread's PCB */
  
	/* Switch to lwp0 context */

	ldr	r9, .Lcpufuncs
#if !defined(CPU_ARM11) && !defined(CPU_CORTEXA) && !defined(CPU_MV_PJ4B) && !defined(CPU_KRAIT)
	mov	lr, pc
	ldr	pc, [r9, #CF_IDCACHE_WBINV_ALL]
#endif
	ldr	r0, [r7, #(PCB_PL1VEC)]
	ldr	r1, [r7, #(PCB_DACR)]
	/*
	 * r0 = Pointer to L1 slot for vector_page (or NULL)
	 * r1 = lwp0's DACR
	 * r5 = lwp0
	 * r6 = exit func
	 * r7 = lwp0's PCB
	 * r9 = cpufuncs
	 */

	/*
	 * Ensure the vector table is accessible by fixing up lwp0's L1
	 */
	cmp	r0, #0			/* No need to fixup vector table? */
	ldrne	r3, [r0]		/* But if yes, fetch current value */
	ldrne	r2, [r7, #(PCB_L1VEC)]	/* Fetch new vector_page value */
	mcr	p15, 0, r1, c3, c0, 0	/* Update DACR for lwp0's context */
	cmpne	r3, r2			/* Stuffing the same value? */
	strne	r2, [r0]		/* Store if not. */

#ifdef PMAP_INCLUDE_PTE_SYNC
	/*
	 * Need to sync the cache to make sure that last store is
	 * visible to the MMU.
	 */
	movne	r1, #4
	movne	lr, pc
	ldrne	pc, [r9, #CF_DCACHE_WB_RANGE]
#endif /* PMAP_INCLUDE_PTE_SYNC */

	/*
	 * Note: We don't do the same optimisation as cpu_switch() with
	 * respect to avoiding flushing the TLB if we're switching to
	 * the same L1 since this process' VM space may be about to go
	 * away, so we don't want *any* turds left in the TLB.
	 */

	/* Switch the memory to the new process */
	ldr	r0, [r7, #(PCB_PAGEDIR)]
	mov	lr, pc
	ldr	pc, [r9, #CF_CONTEXT_SWITCH]

	/* Restore all the save registers */
#ifndef _ARM_ARCH_5E
	add	r1, r7, #PCB_R8
	ldmia	r1, {r8-r13}
#else
	ldr	r8, [r7, #(PCB_R8)]
	ldr	r9, [r7, #(PCB_R9)]
	ldr	r10, [r7, #(PCB_R10)]
	ldr	r11, [r7, #(PCB_R11)]
	ldr	r12, [r7, #(PCB_R12)]
	ldr	r13, [r7, #(PCB_SP)]
#endif

	GET_PCPU(r6, r4)
	/* Hook in a new pcb */
	str	r7, [r6, #PC_CURPCB]
	/* We have a new curthread now so make a note it */
	add	r6, r6, #PC_CURTHREAD
	str	r5, [r6]
#ifndef ARM_TP_ADDRESS
	mcr	p15, 0, r5, c13, c0, 4
#endif
	/* Set the new tp */
	ldr	r6, [r5, #(TD_MD + MD_TP)]
#ifdef ARM_TP_ADDRESS
	ldr	r4, =ARM_TP_ADDRESS
	str	r6, [r4]
	ldr	r6, [r5, #(TD_MD + MD_RAS_START)]
	str	r6, [r4, #4] /* ARM_RAS_START */
	ldr	r6, [r5, #(TD_MD + MD_RAS_END)]
	str	r6, [r4, #8] /* ARM_RAS_END */
#else
	mcr p15, 0, r6, c13, c0, 3
#endif

	add	sp, sp, #4;
	ldmfd	sp!, {r4-r7, pc}
END(cpu_throw)

ENTRY(cpu_switch)
	stmfd	sp!, {r4-r7, lr}
	sub	sp, sp, #4;
#ifdef __ARM_EABI__
	.save	{r4-r7, lr}
	.pad	#4
#endif

	mov	r6, r2 /* Save the mutex */

.Lswitch_resume:
	/* rem: r0 = old lwp */
	/* rem: interrupts are disabled */

	/* Process is now on a processor. */
	/* We have a new curthread now so make a note it */
	GET_PCPU(r7, r2)
	str	r1, [r7, #PC_CURTHREAD]
#ifndef ARM_TP_ADDRESS
	mcr	p15, 0, r1, c13, c0, 4
#endif

	/* Hook in a new pcb */
	ldr	r2, [r1, #TD_PCB]
	str	r2, [r7, #PC_CURPCB]

	/* rem: r1 = new process */
	/* rem: interrupts are enabled */

	/* Stage two : Save old context */

	/* Get the user structure for the old thread. */
	ldr	r2, [r0, #(TD_PCB)]
	mov	r4, r0 /* Save the old thread. */

	/* Save all the registers in the old thread's pcb */
#ifndef _ARM_ARCH_5E
	add	r7, r2, #(PCB_R8)
	stmia	r7, {r8-r13}
#else
	strd	r8, [r2, #(PCB_R8)]
	strd	r10, [r2, #(PCB_R10)]
	strd	r12, [r2, #(PCB_R12)]
#endif
	str	pc, [r2, #(PCB_PC)]

	/*
	 * NOTE: We can now use r8-r13 until it is time to restore
	 * them for the new process.
	 */
#ifdef ARM_TP_ADDRESS
	/* Store the old tp */
	ldr	r3, =ARM_TP_ADDRESS
	ldr	r9, [r3]
	str	r9, [r0, #(TD_MD + MD_TP)]
	ldr	r9, [r3, #4]
	str	r9, [r0, #(TD_MD + MD_RAS_START)]
	ldr	r9, [r3, #8]
	str	r9, [r0, #(TD_MD + MD_RAS_END)]

	/* Set the new tp */
	ldr	r9, [r1, #(TD_MD + MD_TP)]
	str	r9, [r3]
	ldr	r9, [r1, #(TD_MD + MD_RAS_START)]
	str	r9, [r3, #4]
	ldr	r9, [r1, #(TD_MD + MD_RAS_END)]
	str	r9, [r3, #8]
#else
	/* Store the old tp */
	mrc p15, 0, r9, c13, c0, 3
	str	r9, [r0, #(TD_MD + MD_TP)]

	/* Set the new tp */
	ldr	r9, [r1, #(TD_MD + MD_TP)]
	mcr p15, 0, r9, c13, c0, 3
#endif
	
	/* Get the user structure for the new process in r9 */
	ldr	r9, [r1, #(TD_PCB)]

	/* rem: r2 = old PCB */
	/* rem: r9 = new PCB */
	/* rem: interrupts are enabled */

#ifdef VFP
	fmrx	r0, fpexc		/* If the VFP is enabled */
	tst	r0, #(VFPEXC_EN)	/* the current thread has */
	movne	r1, #1			/* used it, so go save */
	addne	r0, r2, #(PCB_VFPSTATE)	/* the state into the PCB */
	blne	_C_LABEL(vfp_store)	/* and disable the VFP. */
#endif

	/* r0-r3 now free! */

	/* Third phase : restore saved context */

	/* rem: r2 = old PCB */
	/* rem: r9 = new PCB */
	/* rem: interrupts are enabled */

	ldr	r5, [r9, #(PCB_DACR)]		/* r5 = new DACR */
	mov	r2, #DOMAIN_CLIENT
	cmp     r5, r2, lsl #(PMAP_DOMAIN_KERNEL * 2) /* Sw to kernel thread? */
	beq     .Lcs_context_switched        /* Yup. Don't flush cache */
	mrc	p15, 0, r0, c3, c0, 0		/* r0 = old DACR */
	/*
	 * Get the new L1 table pointer into r11.  If we're switching to
	 * an LWP with the same address space as the outgoing one, we can
	 * skip the cache purge and the TTB load.
	 *
	 * To avoid data dep stalls that would happen anyway, we try
	 * and get some useful work done in the mean time.
	 */
	mrc	p15, 0, r10, c2, c0, 0		/* r10 = old L1 */
	ldr	r11, [r9, #(PCB_PAGEDIR)]	/* r11 = new L1 */


	teq	r10, r11			/* Same L1? */
	cmpeq	r0, r5				/* Same DACR? */
	beq	.Lcs_context_switched		/* yes! */

#if !defined(CPU_ARM11) && !defined(CPU_CORTEXA) && !defined(CPU_MV_PJ4B) && !defined(CPU_KRAIT)
	/*
	 * Definately need to flush the cache.
	 */

	ldr	r1, .Lcpufuncs
	mov	lr, pc
	ldr	pc, [r1, #CF_IDCACHE_WBINV_ALL]
#endif
.Lcs_cache_purge_skipped:
	/* rem: r6 = lock */
	/* rem: r9 = new PCB */
	/* rem: r10 = old L1 */
	/* rem: r11 = new L1 */

	mov	r2, #0x00000000
	ldr	r7, [r9, #(PCB_PL1VEC)]

	/*
	 * Ensure the vector table is accessible by fixing up the L1
	 */
	cmp	r7, #0			/* No need to fixup vector table? */
	ldrne	r2, [r7]		/* But if yes, fetch current value */
	ldrne	r0, [r9, #(PCB_L1VEC)]	/* Fetch new vector_page value */
	mcr	p15, 0, r5, c3, c0, 0	/* Update DACR for new context */
	cmpne	r2, r0			/* Stuffing the same value? */
#ifndef PMAP_INCLUDE_PTE_SYNC
	strne	r0, [r7]		/* Nope, update it */
#else
	beq	.Lcs_same_vector
	str	r0, [r7]		/* Otherwise, update it */

	/*
	 * Need to sync the cache to make sure that last store is
	 * visible to the MMU.
	 */
	ldr	r2, .Lcpufuncs
	mov	r0, r7
	mov	r1, #4
	mov	lr, pc
	ldr	pc, [r2, #CF_DCACHE_WB_RANGE]

.Lcs_same_vector:
#endif /* PMAP_INCLUDE_PTE_SYNC */

	cmp	r10, r11		/* Switching to the same L1? */
	ldr	r10, .Lcpufuncs
	beq	.Lcs_same_l1		/* Yup. */
	/*
	 * Do a full context switch, including full TLB flush.
	 */
	mov	r0, r11
	mov	lr, pc
	ldr	pc, [r10, #CF_CONTEXT_SWITCH]

	b	.Lcs_context_switched

	/*
	 * We're switching to a different process in the same L1.
	 * In this situation, we only need to flush the TLB for the
	 * vector_page mapping, and even then only if r7 is non-NULL.
	 */
.Lcs_same_l1:
	cmp	r7, #0
	movne	r0, #0			/* We *know* vector_page's VA is 0x0 */
	movne	lr, pc
	ldrne	pc, [r10, #CF_TLB_FLUSHID_SE]

.Lcs_context_switched:

	/* Release the old thread */
	str	r6, [r4, #TD_LOCK]
#if defined(SCHED_ULE) && defined(SMP)
	ldr	r6, .Lblocked_lock
	GET_CURTHREAD_PTR(r3)
1:
	ldr	r4, [r3, #TD_LOCK]
	cmp	r4, r6
	beq	1b
#endif
	
	/* XXXSCW: Safe to re-enable FIQs here */

	/* rem: r9 = new PCB */

	/* Restore all the save registers */
#ifndef _ARM_ARCH_5E
	add	r7, r9, #PCB_R8
	ldmia	r7, {r8-r13}
	sub	r7, r7, #PCB_R8		/* restore PCB pointer */
#else
	mov	r7, r9
	ldr	r8, [r7, #(PCB_R8)]
	ldr	r9, [r7, #(PCB_R9)]
	ldr	r10, [r7, #(PCB_R10)]
	ldr	r11, [r7, #(PCB_R11)]
	ldr	r12, [r7, #(PCB_R12)]
	ldr	r13, [r7, #(PCB_SP)]
#endif

	/* rem: r5 = new lwp's proc */
	/* rem: r6 = lock */
	/* rem: r7 = new PCB */

.Lswitch_return:

	/*
	 * Pull the registers that got pushed when either savectx() or
	 * cpu_switch() was called and return.
	 */
	add	sp, sp, #4;
	ldmfd	sp!, {r4-r7, pc}
#ifdef DIAGNOSTIC
.Lswitch_bogons:
	adr	r0, .Lswitch_panic_str
	bl	_C_LABEL(panic)
1:	nop
	b	1b

.Lswitch_panic_str:
	.asciz	"cpu_switch: sched_qs empty with non-zero sched_whichqs!\n"
#endif
END(cpu_switch)

ENTRY(savectx)
	stmfd   sp!, {r4-r7, lr}
	sub	sp, sp, #4
	/*
	 * r0 = pcb
	 */
	/* Store all the registers in the process's pcb */
	add	r2, r0, #(PCB_R8)
	stmia	r2, {r8-r13}
#ifdef VFP
	fmrx	r2, fpexc		/* If the VFP is enabled */
	tst	r2, #(VFPEXC_EN)	/* the current thread has */
	movne	r1, #1			/* used it, so go save */
	addne	r0, r0, #(PCB_VFPSTATE)	/* the state into the PCB */
	blne	_C_LABEL(vfp_store)	/* and disable the VFP. */
#endif
	add	sp, sp, #4;
	ldmfd	sp!, {r4-r7, pc}
END(savectx)

ENTRY(fork_trampoline)
	STOP_UNWINDING	/* EABI: Don't unwind beyond the thread enty point. */
	mov	fp, #0	/* OABI: Stack traceback via fp stops here. */
	mov	r2, sp
	mov	r1, r5
	mov	r0, r4
	ldr	lr, =swi_exit		/* Go finish forking, then return */
	b	_C_LABEL(fork_exit)	/* to userland via swi_exit code. */
END(fork_trampoline)

