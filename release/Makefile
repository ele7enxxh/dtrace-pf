# $FreeBSD: release/10.1.0/release/Makefile 274104 2014-11-04 23:03:59Z gjb $
#
# Makefile for building releases and release media.
#
# User-driven targets:
#  cdrom: Builds release CD-ROM media (disc1.iso)
#  dvdrom: Builds release DVD-ROM media (dvd1.iso)
#  memstick: Builds memory stick image (memstick.img)
#  mini-memstick: Builds minimal memory stick image (mini-memstick.img)
#  ftp: Sets up FTP distribution area (ftp)
#  release: Build all media and FTP distribution area
#  install: Copies all release media into ${DESTDIR}
#
# Variables affecting the build process:
#  WORLDDIR: location of src tree -- must have built world and default kernel
#            (by default, the directory above this one)
#  PORTSDIR: location of ports tree to distribute (default: /usr/ports)
#  DOCDIR:   location of doc tree (default: /usr/doc)
#  NOPKG:    if set, do not distribute third-party packages
#  NOPORTS:  if set, do not distribute ports tree
#  NOSRC:    if set, do not distribute source tree
#  NODOC:    if set, do not generate release documentation
#  WITH_DVD: if set, generate dvd1.iso
#  WITH_COMPRESSED_IMAGES: if set, compress installation images with xz(1)
#		(uncompressed images are not removed)
#  WITH_VMIMAGES: if set, build virtual machine images with the release
#  WITH_COMPRESSED_VMIMAGES: if set, compress virtual machine disk images
#  		with xz(1) (extremely time consuming)
#  TARGET/TARGET_ARCH: architecture of built release
#

WORLDDIR?=	${.CURDIR}/..
PORTSDIR?=	/usr/ports
DOCDIR?=	/usr/doc
RELNOTES_LANG?= en_US.ISO8859-1
XZCMD?=		/usr/bin/xz

.if !defined(TARGET) || empty(TARGET)
TARGET=		${MACHINE}
.endif
.if !defined(TARGET_ARCH) || empty(TARGET_ARCH)
.if ${TARGET} == ${MACHINE}
TARGET_ARCH=	${MACHINE_ARCH}
.else
TARGET_ARCH=	${TARGET}
.endif
.endif
IMAKE=		${MAKE} TARGET_ARCH=${TARGET_ARCH} TARGET=${TARGET}
DISTDIR=	dist

# Define OSRELEASE by using newvars.sh
.if !defined(OSRELEASE) || empty(OSRELEASE)
.for _V in TYPE BRANCH REVISION
${_V}!=	eval $$(awk '/^${_V}=/{print}' ${.CURDIR}/../sys/conf/newvers.sh); echo $$${_V}
.endfor
.for _V in ${TARGET_ARCH}
.if !empty(TARGET:M${_V})
OSRELEASE=	${TYPE}-${REVISION}-${BRANCH}-${TARGET}
VOLUME_LABEL=	${REVISION:C/[.-]/_/g}_${BRANCH:C/[.-]/_/g}_${TARGET}
.else
OSRELEASE=	${TYPE}-${REVISION}-${BRANCH}-${TARGET}-${TARGET_ARCH}
VOLUME_LABEL=	${REVISION:C/[.-]/_/g}_${BRANCH:C/[.-]/_/g}_${TARGET_ARCH}
.endif
.endfor
.endif

.if !defined(VOLUME_LABEL) || empty(VOLUME_LABEL)
VOLUME_LABEL=	FreeBSD_Install
.endif

.if !exists(${DOCDIR})
NODOC= true
.endif
.if !exists(${PORTSDIR})
NOPORTS= true
.endif

EXTRA_PACKAGES=
.if !defined(NOPORTS)
EXTRA_PACKAGES+= ports.txz
.endif
.if !defined(NOSRC)
EXTRA_PACKAGES+= src.txz
.endif
.if !defined(NODOC)
EXTRA_PACKAGES+= reldoc
.endif

RELEASE_TARGETS= ftp
IMAGES=
.if exists(${.CURDIR}/${TARGET}/mkisoimages.sh)
RELEASE_TARGETS+= cdrom
IMAGES+=	disc1.iso bootonly.iso
. if defined(WITH_DVD) && !empty(WITH_DVD)
RELEASE_TARGETS+= dvdrom
IMAGES+=	dvd1.iso
. endif
.endif
.if exists(${.CURDIR}/${TARGET}/make-memstick.sh)
RELEASE_TARGETS+= memstick.img
RELEASE_TARGETS+= mini-memstick.img
IMAGES+=	memstick.img
IMAGES+=	mini-memstick.img
.endif
.if exists(${.CURDIR}/${TARGET}/make-uefi-memstick.sh)
RELEASE_TARGETS+=uefi-memstick.img
RELEASE_TARGETS+=uefi-mini-memstick.img
IMAGES+=	uefi-memstick.img
IMAGES+=	uefi-mini-memstick.img
.endif
.if exists(${.CURDIR}/${TARGET}/mkisoimages-uefi.sh)
RELEASE_TARGETS+=uefi-disc1.iso
RELEASE_TARGETS+=uefi-bootonly.iso
IMAGES+=	uefi-disc1.iso
IMAGES+=	uefi-bootonly.iso
. if defined(WITH_DVD) && !empty(WITH_DVD)
RELEASE_TARGETS+=uefi-dvd1.iso
IMAGES+=	uefi-dvd1.iso
. endif
.endif

VMTARGETS=	vm-base vm-image
VMFORMATS?=	vhd vmdk qcow2 raw
VMSIZE?=	20G
VMBASE?=	vm
AZURECONF?=	${.CURDIR}/tools/azure.conf

CLEANFILES=	packagesystem *.txz MANIFEST system ${IMAGES}
.if defined(WITH_COMPRESSED_IMAGES) && !empty(WITH_COMPRESSED_IMAGES)
. for I in ${IMAGES}
CLEANFILES+=	${I}.xz
. endfor
.endif
.if defined(WITH_DVD) && !empty(WITH_DVD)
CLEANFILES+=	pkg-stage
.endif
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
CLEANFILES+=	${VMBASE}.img
. for FORMAT in ${VMFORMATS}
CLEANFILES+=	${VMBASE}.${FORMAT}
. endfor
.endif
CLEANDIRS=	dist ftp release bootonly dvd
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
CLEANDIRS+=	${VMTARGETS}
.endif
.if exists(${.CURDIR}/${TARGET}/mk-azure.sh)
CLEANFILES+=	${OSRELEASE}.vhd \
		${OSRELEASE}.vhd.raw \
		azure.img
CLEANDIRS+=	vm-azure
.endif
beforeclean:
	chflags -R noschg .
.include <bsd.obj.mk>
clean: beforeclean

base.txz:
	mkdir -p ${DISTDIR}
	cd ${WORLDDIR} && ${IMAKE} distributeworld DISTDIR=${.OBJDIR}/${DISTDIR}
# Set up mergemaster root database
	sh ${.CURDIR}/scripts/mm-mtree.sh -m ${WORLDDIR} -F \
	    "TARGET_ARCH=${TARGET_ARCH} TARGET=${TARGET}" -D "${.OBJDIR}/${DISTDIR}/base"
	etcupdate extract -B -M "TARGET_ARCH=${TARGET_ARCH} TARGET=${TARGET}" \
	    -s ${WORLDDIR} -d "${.OBJDIR}/${DISTDIR}/base/var/db/etcupdate"
# Package all components
	cd ${WORLDDIR} && ${IMAKE} packageworld DISTDIR=${.OBJDIR}/${DISTDIR}
	mv ${DISTDIR}/*.txz .

kernel.txz:
	mkdir -p ${DISTDIR}
	cd ${WORLDDIR} && ${IMAKE} distributekernel packagekernel DISTDIR=${.OBJDIR}/${DISTDIR}
	mv ${DISTDIR}/kernel*.txz .

src.txz:
	mkdir -p ${DISTDIR}/usr
	ln -fs ${WORLDDIR} ${DISTDIR}/usr/src
	cd ${DISTDIR} && tar cLvJf ${.OBJDIR}/src.txz --exclude .svn --exclude .zfs \
	    --exclude .git --exclude @ --exclude usr/src/release/dist usr/src

ports.txz:
	mkdir -p ${DISTDIR}/usr
	ln -fs ${PORTSDIR} ${DISTDIR}/usr/ports
	cd ${DISTDIR} && tar cLvJf ${.OBJDIR}/ports.txz \
	    --exclude .git --exclude .svn \
	    --exclude usr/ports/distfiles --exclude usr/ports/packages \
	    --exclude 'usr/ports/INDEX*' --exclude work usr/ports

reldoc:
	cd ${.CURDIR}/doc && ${MAKE} all install clean 'FORMATS=html txt' \
	    INSTALL_COMPRESSED='' URLS_ABSOLUTE=YES DOCDIR=${.OBJDIR}/rdoc
	mkdir -p reldoc
.for i in hardware readme relnotes errata
	ln -f rdoc/${RELNOTES_LANG}/${i}/article.txt reldoc/${i:tu}.TXT
	ln -f rdoc/${RELNOTES_LANG}/${i}/article.html reldoc/${i:tu}.HTM
.endfor
	cp rdoc/${RELNOTES_LANG}/readme/docbook.css reldoc

system: packagesystem
# Install system
	mkdir -p release
	cd ${WORLDDIR} && ${IMAKE} installkernel installworld distribution \
		DESTDIR=${.OBJDIR}/release WITHOUT_RESCUE=1 WITHOUT_KERNEL_SYMBOLS=1 \
		WITHOUT_PROFILE=1 WITHOUT_SENDMAIL=1 WITHOUT_ATF=1 WITHOUT_LIB32=1
# Copy distfiles
	mkdir -p release/usr/freebsd-dist
	cp *.txz MANIFEST release/usr/freebsd-dist
# Copy documentation, if generated
.if !defined(NODOC)
	cp reldoc/* release
.endif
# Set up installation environment
	ln -fs /tmp/bsdinstall_etc/resolv.conf release/etc/resolv.conf
	echo sendmail_enable=\"NONE\" > release/etc/rc.conf
	echo hostid_enable=\"NO\" >> release/etc/rc.conf
	cp ${.CURDIR}/rc.local release/etc
	touch ${.TARGET}

bootonly: packagesystem
# Install system
	mkdir -p bootonly
	cd ${WORLDDIR} && ${IMAKE} installkernel installworld distribution \
	    DESTDIR=${.OBJDIR}/bootonly WITHOUT_AMD=1 WITHOUT_AT=1 \
	    WITHOUT_GAMES=1 WITHOUT_GROFF=1 \
	    WITHOUT_INSTALLLIB=1 WITHOUT_LIB32=1 WITHOUT_MAIL=1 \
	    WITHOUT_NCP=1 WITHOUT_TOOLCHAIN=1 WITHOUT_PROFILE=1 \
	    WITHOUT_INSTALLIB=1 WITHOUT_RESCUE=1 WITHOUT_DICT=1 \
	    WITHOUT_KERNEL_SYMBOLS=1
# Copy manifest only (no distfiles) to get checksums
	mkdir -p bootonly/usr/freebsd-dist
	cp MANIFEST bootonly/usr/freebsd-dist
# Copy documentation, if generated
.if !defined(NODOC)
	cp reldoc/* bootonly
.endif
# Set up installation environment
	ln -fs /tmp/bsdinstall_etc/resolv.conf bootonly/etc/resolv.conf
	echo sendmail_enable=\"NONE\" > bootonly/etc/rc.conf
	echo hostid_enable=\"NO\" >> bootonly/etc/rc.conf
	cp ${.CURDIR}/rc.local bootonly/etc

dvd:
# Install system
	mkdir -p ${.TARGET}
	cd ${WORLDDIR} && ${IMAKE} installkernel installworld distribution \
		DESTDIR=${.OBJDIR}/${.TARGET} WITHOUT_RESCUE=1 WITHOUT_KERNEL_SYMBOLS=1
# Copy distfiles
	mkdir -p ${.TARGET}/usr/freebsd-dist
	cp *.txz MANIFEST ${.TARGET}/usr/freebsd-dist
# Copy documentation, if generated
.if !defined(NODOC)
	cp reldoc/* ${.TARGET}
.endif
# Set up installation environment
	ln -fs /tmp/bsdinstall_etc/resolv.conf ${.TARGET}/etc/resolv.conf
	echo sendmail_enable=\"NONE\" > ${.TARGET}/etc/rc.conf
	echo hostid_enable=\"NO\" >> ${.TARGET}/etc/rc.conf
	cp ${.CURDIR}/rc.local ${.TARGET}/etc
	touch ${.TARGET}

release.iso: disc1.iso
disc1.iso: system
	sh ${.CURDIR}/${TARGET}/mkisoimages.sh -b ${VOLUME_LABEL}_CD ${.TARGET} release

uefi-disc1.iso: system
.if exists(${.CURDIR}/${TARGET}/mkisoimages-uefi.sh)
	sh ${.CURDIR}/${TARGET}/mkisoimages-uefi.sh -b ${VOLUME_LABEL}_UEFICD \
		${.TARGET} release
.endif

uefi-bootonly.iso: bootonly
.if exists(${.CURDIR}/${TARGET}/mkisoimages-uefi.sh)
	sh ${.CURDIR}/${TARGET}/mkisoimages-uefi.sh -b ${VOLUME_LABEL}_UEFIBO \
		${.TARGET} bootonly
.endif

dvd1.iso: dvd pkg-stage
	sh ${.CURDIR}/${TARGET}/mkisoimages.sh -b ${VOLUME_LABEL}_DVD ${.TARGET} dvd

uefi-dvd1.iso: dvd pkg-stage
.if exists(${.CURDIR}/${TARGET}/mkisoimages-uefi.sh)
	sh ${.CURDIR}/${TARGET}/mkisoimages-uefi.sh -b ${VOLUME_LABEL}_UEFIDVD \
		${.TARGET} dvd
.endif

bootonly.iso: bootonly
	sh ${.CURDIR}/${TARGET}/mkisoimages.sh -b ${VOLUME_LABEL}_BO ${.TARGET} bootonly

memstick: memstick.img
memstick.img: system
	sh ${.CURDIR}/${TARGET}/make-memstick.sh release ${.TARGET}

mini-memstick: mini-memstick.img
mini-memstick.img: system
	sh ${.CURDIR}/${TARGET}/make-memstick.sh bootonly ${.TARGET}

uefi-memstick: uefi-memstick.img
uefi-memstick.img: system
.if exists(${.CURDIR}/${TARGET}/make-uefi-memstick.sh)
	sh ${.CURDIR}/${TARGET}/make-uefi-memstick.sh release ${.TARGET}
.endif

uefi-mini-memstick: uefi-mini-memstick.img
uefi-mini-memstick.img: system
.if exists(${.CURDIR}/${TARGET}/make-uefi-memstick.sh)
	sh ${.CURDIR}/${TARGET}/make-uefi-memstick.sh bootonly ${.TARGET}
.endif

packagesystem: base.txz kernel.txz ${EXTRA_PACKAGES}
	sh ${.CURDIR}/scripts/make-manifest.sh *.txz > MANIFEST
	touch ${.TARGET}

pkg-stage:
.if !defined(NOPKG)
	env REPOS_DIR=${.CURDIR}/pkg_repos/ \
		sh ${.CURDIR}/scripts/pkg-stage.sh
	mkdir -p ${.OBJDIR}/dvd/packages/repos/
	cp ${.CURDIR}/scripts/FreeBSD_install_cdrom.conf \
		${.OBJDIR}/dvd/packages/repos/
.endif
	touch ${.TARGET}

cdrom: disc1.iso bootonly.iso
dvdrom: dvd1.iso
ftp: packagesystem
	rm -rf ftp
	mkdir -p ftp
	cp *.txz MANIFEST ftp

release:
	${MAKE} -C ${.CURDIR} ${.MAKEFLAGS} obj
	${MAKE} -C ${.CURDIR} ${.MAKEFLAGS} ${RELEASE_TARGETS}
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
	${MAKE} -C ${.CURDIR} ${.MAKEFLAGS} ${VMTARGETS}
.endif

install:
.if defined(DESTDIR) && !empty(DESTDIR)
	mkdir -p ${DESTDIR}
.endif
	cp -a ftp ${DESTDIR}/
.for I in ${IMAGES}
	cp -p ${I} ${DESTDIR}/${OSRELEASE}-${I}
. if defined(WITH_COMPRESSED_IMAGES) && !empty(WITH_COMPRESSED_IMAGES)
	${XZCMD} -k ${DESTDIR}/${OSRELEASE}-${I}
. endif
.endfor
	cd ${DESTDIR} && sha256 ${OSRELEASE}* > ${DESTDIR}/CHECKSUM.SHA256
	cd ${DESTDIR} && md5 ${OSRELEASE}* > ${DESTDIR}/CHECKSUM.MD5
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
	mkdir -p ${DESTDIR}/vmimages
. for FORMAT in ${VMFORMATS}
	cp -p ${VMBASE}.${FORMAT} \
		${DESTDIR}/vmimages/${OSRELEASE}.${FORMAT}
. endfor
. if defined(WITH_COMPRESSED_VMIMAGES) && !empty(WITH_COMPRESSED_VMIMAGES)
# This is very time consuming, so defer it after the images are moved to
# the DESTDIR.
.  for FORMAT in ${VMFORMATS}
	# Don't keep the originals.  There is a copy in ${.OBJDIR} if needed.
	${XZCMD} ${DESTDIR}/vmimages/${OSRELEASE}.${FORMAT}
.  endfor
. endif
	cd ${DESTDIR}/vmimages && sha256 ${OSRELEASE}* > \
		${DESTDIR}/vmimages/CHECKSUM.SHA256
	cd ${DESTDIR}/vmimages && md5 ${OSRELEASE}* > \
		${DESTDIR}/vmimages/CHECKSUM.MD5
.endif

vm-base:
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
. if exists(${.CURDIR}/${TARGET}/mk-vmimage.sh)
	env TARGET=${TARGET} TARGET_ARCH=${TARGET_ARCH} \
		${.CURDIR}/${TARGET}/mk-vmimage.sh ${.TARGET} \
		${VMBASE}.img ${WORLDDIR} ${.OBJDIR}/${.TARGET} ${VMSIZE}
. endif
.endif
	touch ${.TARGET}

vm-image: vm-base
.if defined(WITH_VMIMAGES) && !empty(WITH_VMIMAGES)
. if exists(${.CURDIR}/${TARGET}/mk-vmimage.sh)
.  for FORMAT in ${VMFORMATS}
	env TARGET=${TARGET} TARGET_ARCH=${TARGET_ARCH} \
		${.CURDIR}/${TARGET}/mk-vmimage.sh ${.TARGET} \
		${VMBASE}.img ${FORMAT} ${VMBASE}.${FORMAT}
.  endfor
. endif
.endif
	touch ${.TARGET}

vm-azure:
.if exists(${.CURDIR}/${TARGET}/mk-azure.sh)
	env TARGET=${TARGET} TARGET_ARCH=${TARGET_ARCH} AZURECONF=${AZURECONF} \
		${.CURDIR}/${TARGET}/mk-azure.sh ${.TARGET} azure.img \
		${WORLDDIR} ${.TARGET} ${VMSIZE} ${OSRELEASE}.vhd
.endif
	touch ${.TARGET}
