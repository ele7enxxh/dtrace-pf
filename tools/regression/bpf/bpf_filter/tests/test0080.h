/*-
 * Test 0080:	Check uninitialized scratch memory (obsolete).
 *
 * $FreeBSD: release/10.1.0/tools/regression/bpf/bpf_filter/tests/test0080.h 199604 2009-11-20 18:53:38Z jkim $
 */

/* BPF program */
struct bpf_insn pc[] = {
#ifdef BPF_JIT_COMPILER_OBSOLETE
	BPF_STMT(BPF_LDX+BPF_IMM, 0xffffffff),
	BPF_STMT(BPF_LD+BPF_MEM, 0),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 29, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 1),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 27, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 2),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 25, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 3),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 23, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 4),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 21, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 5),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 19, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 6),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 17, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 7),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 15, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 8),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 13, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 9),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 11, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 10),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 9, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 11),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 7, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 12),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 5, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 13),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 3, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 14),
	BPF_JUMP(BPF_JMP+BPF_JSET+BPF_X, 0, 1, 0),
	BPF_STMT(BPF_LD+BPF_MEM, 15),
#else
	BPF_STMT(BPF_LD+BPF_IMM, 0),
#endif
	BPF_STMT(BPF_RET+BPF_A, 0),
};

/* Packet */
u_char	pkt[] = {
	0x00,
};

/* Packet length seen on wire */
u_int	wirelen =	sizeof(pkt);

/* Packet length passed on buffer */
u_int	buflen =	sizeof(pkt);

/* Invalid instruction */
int	invalid =	0;

/* Expected return value */
u_int	expect =	0;

/* Expected signal */
int	expect_signal =	0;
