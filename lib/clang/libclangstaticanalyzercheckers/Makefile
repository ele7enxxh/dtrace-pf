# $FreeBSD: release/10.1.0/lib/clang/libclangstaticanalyzercheckers/Makefile 263508 2014-03-21 17:53:59Z dim $

.include <bsd.own.mk>

LIB=	clangstaticanalyzercheckers

SRCDIR=	tools/clang/lib/StaticAnalyzer/Checkers
SRCS=	AllocationDiagnostics.cpp \
	AnalyzerStatsChecker.cpp \
	ArrayBoundChecker.cpp \
	ArrayBoundCheckerV2.cpp \
	BasicObjCFoundationChecks.cpp \
	BoolAssignmentChecker.cpp \
	BuiltinFunctionChecker.cpp \
	CStringChecker.cpp \
	CStringSyntaxChecker.cpp \
	CallAndMessageChecker.cpp \
	CastSizeChecker.cpp \
	CastToStructChecker.cpp \
	CheckObjCDealloc.cpp \
	CheckObjCInstMethSignature.cpp \
	CheckSecuritySyntaxOnly.cpp \
	CheckSizeofPointer.cpp \
	CheckerDocumentation.cpp \
	ChrootChecker.cpp \
	ClangCheckers.cpp \
	DeadStoresChecker.cpp \
	DebugCheckers.cpp \
	DereferenceChecker.cpp \
	DirectIvarAssignment.cpp \
	DivZeroChecker.cpp \
	DynamicTypePropagation.cpp \
	ExprInspectionChecker.cpp \
	FixedAddressChecker.cpp \
	GenericTaintChecker.cpp \
	IdempotentOperationChecker.cpp \
	IdenticalExprChecker.cpp \
	IvarInvalidationChecker.cpp \
	LLVMConventionsChecker.cpp \
	MacOSKeychainAPIChecker.cpp \
	MacOSXAPIChecker.cpp \
	MallocChecker.cpp \
	MallocOverflowSecurityChecker.cpp \
	MallocSizeofChecker.cpp \
	NSAutoreleasePoolChecker.cpp \
	NSErrorChecker.cpp \
	NoReturnFunctionChecker.cpp \
	NonNullParamChecker.cpp \
	ObjCAtSyncChecker.cpp \
	ObjCContainersASTChecker.cpp \
	ObjCContainersChecker.cpp \
	ObjCMissingSuperCallChecker.cpp \
	ObjCSelfInitChecker.cpp \
	ObjCUnusedIVarsChecker.cpp \
	PointerArithChecker.cpp \
	PointerSubChecker.cpp \
	PthreadLockChecker.cpp \
	RetainCountChecker.cpp \
	ReturnPointerRangeChecker.cpp \
	ReturnUndefChecker.cpp \
	SimpleStreamChecker.cpp \
	StackAddrEscapeChecker.cpp \
	StreamChecker.cpp \
	TaintTesterChecker.cpp \
	TraversalChecker.cpp \
	UndefBranchChecker.cpp \
	UndefCapturedBlockVarChecker.cpp \
	UndefResultChecker.cpp \
	UndefinedArraySubscriptChecker.cpp \
	UndefinedAssignmentChecker.cpp \
	UnixAPIChecker.cpp \
	UnreachableCodeChecker.cpp \
	VLASizeChecker.cpp \
	VirtualCallChecker.cpp

TGHDRS=	AttrList \
	Attrs \
	Checkers \
	CommentCommandList \
	CommentNodes \
	DeclNodes \
	DiagnosticCommonKinds \
	StmtNodes

.include "../clang.lib.mk"
