# $FreeBSD: release/10.1.0/lib/clang/libllvmsparcasmparser/Makefile 262263 2014-02-20 22:22:39Z dim $

.include <bsd.own.mk>

LIB=	llvmsparcasmparser

SRCDIR=	lib/Target/Sparc/AsmParser
INCDIR=	lib/Target/Sparc
SRCS=	SparcAsmParser.cpp

TGHDRS=	SparcGenAsmMatcher \
	SparcGenInstrInfo \
	SparcGenRegisterInfo \
	SparcGenSubtargetInfo

.include "../clang.lib.mk"
