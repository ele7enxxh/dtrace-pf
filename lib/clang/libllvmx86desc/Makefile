# $FreeBSD: release/10.1.0/lib/clang/libllvmx86desc/Makefile 263508 2014-03-21 17:53:59Z dim $

.include <bsd.own.mk>

LIB=	llvmx86desc

SRCDIR=	lib/Target/X86/MCTargetDesc
SRCS=	X86AsmBackend.cpp \
	X86ELFObjectWriter.cpp \
	X86ELFRelocationInfo.cpp \
	X86MCAsmInfo.cpp \
	X86MCCodeEmitter.cpp \
	X86MCTargetDesc.cpp \
	X86MachORelocationInfo.cpp \
	X86MachObjectWriter.cpp \
	X86WinCOFFObjectWriter.cpp
CFLAGS+= -I${LLVM_SRCS}/${SRCDIR}/..

TGHDRS=	X86GenInstrInfo \
	X86GenRegisterInfo \
	X86GenSubtargetInfo

.include "../clang.lib.mk"
