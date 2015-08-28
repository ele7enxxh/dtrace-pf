/*-
 * Copyright (C) 1994 by Rodney W. Grimes, Milwaukie, Oregon  97222
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Rodney W. Grimes.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RODNEY W. GRIMES ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL RODNEY W. GRIMES BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: release/10.1.0/sys/mips/include/bootinfo.h 202175 2010-01-12 21:36:08Z imp $
 */

#ifndef	_MACHINE_BOOTINFO_H_
#define	_MACHINE_BOOTINFO_H_

/* Only change the version number if you break compatibility. */
#define	BOOTINFO_VERSION	1

#define	N_BIOS_GEOM		8

#define	MIPS_BOOTINFO_MAGIC	0xCDEACDEA

/* Extended OLV bootinfo struct.  The data area includes a list of named
   OIDs and associated data values.  The format is:

   NUL-terminated dotted-string name
   2 byte length, in big-endian order
   LENGTH bytes of data
   [...]

   The two magic fields are used to guard against other bootloaders that
   may place other sorts of data here.  */

struct bootinfo_ext {
#define	BOOTINFO_EXT_MAGIC1	0x55aa00ff
	unsigned int		magic1;
	unsigned char		*data;
	unsigned int		size;
#define	BOOTINFO_EXT_MAGIC2	0x32719187
	unsigned int		magic2;
};

#define	BOOTINFO_EXT_MAX_SIZE	16384

/*
 * A zero bootinfo field often means that there is no info available.
 * Flags are used to indicate the validity of fields where zero is a
 * normal value.
 */
struct bootinfo {
	u_int32_t	bi_version;
	u_int32_t	bi_kernelname;		/* represents a char * */
	u_int32_t	bi_nfs_diskless;	/* struct nfs_diskless * */
				/* End of fields that are always present. */
#define	bi_endcommon	bi_n_bios_used
	u_int32_t	bi_n_bios_used;
	u_int32_t	bi_bios_geom[N_BIOS_GEOM];
	u_int32_t	bi_size;
	u_int8_t	bi_memsizes_valid;
	u_int8_t	bi_bios_dev;		/* bootdev BIOS unit number */
	u_int8_t	bi_pad[2];
	u_int32_t	bi_basemem;
	u_int32_t	bi_extmem;
	u_int32_t	bi_symtab;		/* struct symtab * */
	u_int32_t	bi_esymtab;		/* struct symtab * */
				/* Items below only from advanced bootloader */
	u_int32_t	bi_kernend;		/* end of kernel space */
	u_int32_t	bi_envp;		/* environment */
	u_int32_t	bi_modulep;		/* preloaded modules */
};

#ifdef _KERNEL
extern struct bootinfo	bootinfo;
#endif

/*
 * Constants for converting boot-style device number to type,
 * adaptor (uba, mba, etc), unit number and partition number.
 * Type (== major device number) is in the low byte
 * for backward compatibility.  Except for that of the "magic
 * number", each mask applies to the shifted value.
 * Format:
 *	 (4) (4) (4) (4)  (8)     (8)
 *	--------------------------------
 *	|MA | AD| CT| UN| PART  | TYPE |
 *	--------------------------------
 */
#define	B_ADAPTORSHIFT		24
#define	B_ADAPTORMASK		0x0f
#define	B_ADAPTOR(val)		(((val) >> B_ADAPTORSHIFT) & B_ADAPTORMASK)
#define	B_CONTROLLERSHIFT	20
#define	B_CONTROLLERMASK	0xf
#define	B_CONTROLLER(val)	(((val)>>B_CONTROLLERSHIFT) & B_CONTROLLERMASK)
#define	B_SLICESHIFT		20
#define	B_SLICEMASK		0xff
#define	B_SLICE(val)		(((val)>>B_SLICESHIFT) & B_SLICEMASK)
#define	B_UNITSHIFT		16
#define	B_UNITMASK		0xf
#define	B_UNIT(val)		(((val) >> B_UNITSHIFT) & B_UNITMASK)
#define	B_PARTITIONSHIFT	8
#define	B_PARTITIONMASK		0xff
#define	B_PARTITION(val)	(((val) >> B_PARTITIONSHIFT) & B_PARTITIONMASK)
#define	B_TYPESHIFT		0
#define	B_TYPEMASK		0xff
#define	B_TYPE(val)		(((val) >> B_TYPESHIFT) & B_TYPEMASK)

#define	B_MAGICMASK	0xf0000000
#define	B_DEVMAGIC	0xa0000000

#define	MAKEBOOTDEV(type, adaptor, controller, unit, partition)		\
	(((type) << B_TYPESHIFT) | ((adaptor) << B_ADAPTORSHIFT) |	\
	((controller) << B_CONTROLLERSHIFT) | ((unit) << B_UNITSHIFT) |	\
	((partition) << B_PARTITIONSHIFT) | B_DEVMAGIC)

#define	BASE_SLICE		2
#define	COMPATIBILITY_SLICE	0
#define	MAX_SLICES		32
#define	WHOLE_DISK_SLICE	1

#endif	/* !_MACHINE_BOOTINFO_H_ */
