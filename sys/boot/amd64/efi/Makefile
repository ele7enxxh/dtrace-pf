# $FreeBSD: release/10.1.0/sys/boot/amd64/efi/Makefile 271135 2014-09-04 21:01:10Z emaste $

NO_MAN=

.include <bsd.own.mk>

# In-tree GCC does not support __attribute__((ms_abi)).
.if ${COMPILER_TYPE} != "gcc"

MK_SSP=		no

PROG=		loader.sym
INTERNALPROG=

# architecture-specific loader code
SRCS=	autoload.c \
	bootinfo.c \
	conf.c \
	copy.c \
	devicename.c \
	elf64_freebsd.c \
	framebuffer.c \
	main.c \
	reloc.c \
	vers.c
SRCS+=	amd64_tramp.S \
	start.S

CFLAGS+=	-fPIC
CFLAGS+=	-I.
CFLAGS+=	-I${.CURDIR}/../../efi/include
CFLAGS+=	-I${.CURDIR}/../../efi/include/${MACHINE_CPUARCH}
CFLAGS+=	-I${.CURDIR}/../../../contrib/dev/acpica/include
CFLAGS+=	-I${.CURDIR}/../../..

.if ${MK_FORTH} != "no"
BOOT_FORTH=	yes
CFLAGS+=	-DBOOT_FORTH
CFLAGS+=	-I${.CURDIR}/../../ficl
CFLAGS+=	-I${.CURDIR}/../../ficl/${MACHINE_CPUARCH}
LIBFICL=	${.OBJDIR}/../../ficl/libficl.a
.endif

# Include bcache code.
HAVE_BCACHE=    yes

# Always add MI sources 
.PATH:		${.CURDIR}/../../common
.include	"${.CURDIR}/../../common/Makefile.inc"
CFLAGS+=	-I${.CURDIR}/../../common

FILES=	loader.efi
FILESMODE_loader.efi=	${BINMODE}

LDSCRIPT=	${.CURDIR}/ldscript.${MACHINE_CPUARCH}
LDFLAGS=	-Wl,-T${LDSCRIPT} -Wl,-Bsymbolic -shared -Wl,-znocombreloc

${PROG}:	${LDSCRIPT}

CLEANFILES=	vers.c loader.efi

NEWVERSWHAT=	"EFI loader" ${MACHINE_CPUARCH}

vers.c:	${.CURDIR}/../../common/newvers.sh ${.CURDIR}/version
	sh ${.CURDIR}/../../common/newvers.sh ${.CURDIR}/version ${NEWVERSWHAT}

OBJCOPY?=	objcopy
OBJDUMP?=	objdump

.if ${MACHINE_CPUARCH} == "amd64"
EFI_TARGET=	efi-app-x86_64
.else
EFI_TARGET=	efi-app-ia32
.endif

loader.efi: loader.sym
	if [ `${OBJDUMP} -t ${.ALLSRC} | fgrep '*UND*' | wc -l` != 0 ]; then \
		${OBJDUMP} -t ${.ALLSRC} | fgrep '*UND*'; \
		exit 1; \
	fi
	${OBJCOPY} -j .text -j .sdata -j .data \
		-j .dynamic -j .dynsym -j .rel.dyn \
		-j .rela.dyn -j .reloc -j .eh_frame -j set_Xcommand_set \
		--target=${EFI_TARGET} ${.ALLSRC} ${.TARGET}

LIBEFI=		${.OBJDIR}/../../efi/libefi/libefi.a
CFLAGS+=	-I${.CURDIR}/../../common

DPADD=		${LIBFICL} ${LIBEFI} ${LIBSTAND}
LDADD=		${LIBFICL} ${LIBEFI} ${LIBSTAND}

.endif # ${COMPILER_TYPE} != "gcc"

.include <bsd.prog.mk>

beforedepend ${OBJS}: machine x86

CLEANFILES+=   machine x86

machine:
	ln -sf ${.CURDIR}/../../../amd64/include machine

x86:
	ln -sf ${.CURDIR}/../../../x86/include x86
