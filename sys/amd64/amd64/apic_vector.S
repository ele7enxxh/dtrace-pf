/*-
 * Copyright (c) 1989, 1990 William F. Jolitz.
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: vector.s, 386BSD 0.1 unknown origin
 * $FreeBSD: release/10.1.0/sys/amd64/amd64/apic_vector.S 264118 2014-04-04 14:54:54Z royger $
 */

/*
 * Interrupt entry points for external interrupts triggered by I/O APICs
 * as well as IPI handlers.
 */

#include "opt_smp.h"

#include <machine/asmacros.h>
#include <x86/apicreg.h>

#include "assym.s"

#ifdef SMP
#define LK	lock ;
#else
#define LK
#endif

/*
 * I/O Interrupt Entry Point.  Rather than having one entry point for
 * each interrupt source, we use one entry point for each 32-bit word
 * in the ISR.  The handler determines the highest bit set in the ISR,
 * translates that into a vector, and passes the vector to the
 * lapic_handle_intr() function.
 */
#define	ISR_VEC(index, vec_name)					\
	.text ;								\
	SUPERALIGN_TEXT ;						\
IDTVEC(vec_name) ;							\
	PUSH_FRAME ;							\
	FAKE_MCOUNT(TF_RIP(%rsp)) ;					\
	movq	lapic, %rdx ;	/* pointer to local APIC */		\
	movl	LA_ISR + 16 * (index)(%rdx), %eax ;	/* load ISR */	\
	bsrl	%eax, %eax ;	/* index of highest set bit in ISR */	\
	jz	1f ;							\
	addl	$(32 * index),%eax ;					\
	movq	%rsp, %rsi	;                                       \
	movl	%eax, %edi ;	/* pass the IRQ */			\
	call	lapic_handle_intr ;					\
1: ;									\
	MEXITCOUNT ;							\
	jmp	doreti

/*
 * Handle "spurious INTerrupts".
 * Notes:
 *  This is different than the "spurious INTerrupt" generated by an
 *   8259 PIC for missing INTs.  See the APIC documentation for details.
 *  This routine should NOT do an 'EOI' cycle.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(spuriousint)

	/* No EOI cycle used here */

	jmp	doreti_iret

	ISR_VEC(1, apic_isr1)
	ISR_VEC(2, apic_isr2)
	ISR_VEC(3, apic_isr3)
	ISR_VEC(4, apic_isr4)
	ISR_VEC(5, apic_isr5)
	ISR_VEC(6, apic_isr6)
	ISR_VEC(7, apic_isr7)

/*
 * Local APIC periodic timer handler.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(timerint)
	PUSH_FRAME
	FAKE_MCOUNT(TF_RIP(%rsp))
	movq	%rsp, %rdi
	call	lapic_handle_timer
	MEXITCOUNT
	jmp	doreti

/*
 * Local APIC CMCI handler.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(cmcint)
	PUSH_FRAME
	FAKE_MCOUNT(TF_RIP(%rsp))
	call	lapic_handle_cmc
	MEXITCOUNT
	jmp	doreti

/*
 * Local APIC error interrupt handler.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(errorint)
	PUSH_FRAME
	FAKE_MCOUNT(TF_RIP(%rsp))
	call	lapic_handle_error
	MEXITCOUNT
	jmp	doreti

#ifdef XENHVM
/*
 * Xen event channel upcall interrupt handler.
 * Only used when the hypervisor supports direct vector callbacks.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(xen_intr_upcall)
	PUSH_FRAME
	FAKE_MCOUNT(TF_RIP(%rsp))
	movq	%rsp, %rdi
	call	xen_intr_handle_upcall
	MEXITCOUNT
	jmp	doreti
#endif

#ifdef SMP
/*
 * Global address space TLB shootdown.
 */
	.text

#define	NAKE_INTR_CS	24

	SUPERALIGN_TEXT
invltlb_ret:
	movq	lapic, %rax
	movl	$0, LA_EOI(%rax)	/* End Of Interrupt to APIC */
	POP_FRAME
	jmp	doreti_iret

	SUPERALIGN_TEXT
IDTVEC(invltlb_pcid)
	PUSH_FRAME

	call	invltlb_pcid_handler
	jmp	invltlb_ret


	SUPERALIGN_TEXT
IDTVEC(invltlb)
	PUSH_FRAME

	call	invltlb_handler
	jmp	invltlb_ret

/*
 * Single page TLB shootdown
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(invlpg_pcid)
	PUSH_FRAME

	call	invlpg_pcid_handler
	jmp	invltlb_ret

	SUPERALIGN_TEXT
IDTVEC(invlpg)
	PUSH_FRAME

	call	invlpg_handler
	jmp	invltlb_ret

/*
 * Page range TLB shootdown.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(invlrng)
	PUSH_FRAME

	call	invlrng_handler
	jmp	invltlb_ret

/*
 * Invalidate cache.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(invlcache)
	PUSH_FRAME

	call	invlcache_handler
	jmp	invltlb_ret

/*
 * Handler for IPIs sent via the per-cpu IPI bitmap.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(ipi_intr_bitmap_handler)		
	PUSH_FRAME

	movq	lapic, %rdx
	movl	$0, LA_EOI(%rdx)	/* End Of Interrupt to APIC */
	
	FAKE_MCOUNT(TF_RIP(%rsp))

	call	ipi_bitmap_handler
	MEXITCOUNT
	jmp	doreti

/*
 * Executed by a CPU when it receives an IPI_STOP from another CPU.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(cpustop)
	PUSH_FRAME

	movq	lapic, %rax
	movl	$0, LA_EOI(%rax)	/* End Of Interrupt to APIC */

	call	cpustop_handler
	jmp	doreti

/*
 * Executed by a CPU when it receives an IPI_SUSPEND from another CPU.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(cpususpend)
	PUSH_FRAME

	call	cpususpend_handler
	movq	lapic, %rax
	movl	$0, LA_EOI(%rax)	/* End Of Interrupt to APIC */
	jmp	doreti

/*
 * Executed by a CPU when it receives a RENDEZVOUS IPI from another CPU.
 *
 * - Calls the generic rendezvous action function.
 */
	.text
	SUPERALIGN_TEXT
IDTVEC(rendezvous)
	PUSH_FRAME
#ifdef COUNT_IPIS
	movl	PCPU(CPUID), %eax
	movq	ipi_rendezvous_counts(,%rax,8), %rax
	incq	(%rax)
#endif
	call	smp_rendezvous_action
	movq	lapic, %rax
	movl	$0, LA_EOI(%rax)	/* End Of Interrupt to APIC */
	jmp	doreti
#endif /* SMP */
