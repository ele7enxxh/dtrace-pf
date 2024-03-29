/*-
 * Copyright (c) 2011 Nathan Whitehorn
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
 *
 * $FreeBSD: release/10.1.0/usr.sbin/bsdinstall/partedit/partedit_x86.c 271637 2014-09-15 17:56:55Z emaste $
 */

#include <sys/types.h>
#include <sys/sysctl.h>
#include <string.h>

#include "partedit.h"

static char platform[255] = "";
static const char *platform_sysctl = "machdep.bootmethod";

const char *
default_scheme(void) {
	return ("GPT");
}

int
is_scheme_bootable(const char *part_type) {
	size_t platlen = sizeof(platform);
	if (strlen(platform) == 0)
		sysctlbyname(platform_sysctl, platform, &platlen, NULL, -1);

	if (strcmp(part_type, "GPT") == 0)
		return (1);
	if (strcmp(platform, "BIOS") == 0) {
		if (strcmp(part_type, "BSD") == 0)
			return (1);
		if (strcmp(part_type, "MBR") == 0)
			return (1);
	}

	return (0);
}

size_t
bootpart_size(const char *scheme) {
	size_t platlen = sizeof(platform);
	if (strlen(platform) == 0)
		sysctlbyname(platform_sysctl, platform, &platlen, NULL, -1);

	/* No partcode except for GPT */
	if (strcmp(scheme, "GPT") != 0)
		return (0);

	if (strcmp(platform, "BIOS") == 0)
		return (512*1024);
	else 
		return (800*1024);

	return (0);
}

const char *
bootpart_type(const char *scheme) {
	size_t platlen = sizeof(platform);
	if (strlen(platform) == 0)
		sysctlbyname(platform_sysctl, platform, &platlen, NULL, -1);

	if (strcmp(platform, "UEFI") == 0)
		return ("efi");

	return ("freebsd-boot");
}

const char *
bootcode_path(const char *part_type) {
	size_t platlen = sizeof(platform);
	if (strlen(platform) == 0)
		sysctlbyname(platform_sysctl, platform, &platlen, NULL, -1);
	if (strcmp(platform, "UEFI") == 0)
		return (NULL);

	if (strcmp(part_type, "GPT") == 0)
		return ("/boot/pmbr");
	if (strcmp(part_type, "MBR") == 0)
		return ("/boot/mbr");
	if (strcmp(part_type, "BSD") == 0)
		return ("/boot/boot");

	return (NULL);
}
	
const char *
partcode_path(const char *part_type) {
	size_t platlen = sizeof(platform);
	if (strlen(platform) == 0)
		sysctlbyname(platform_sysctl, platform, &platlen, NULL, -1);

	if (strcmp(part_type, "GPT") == 0) {
		if (strcmp(platform, "UEFI") == 0)
			return ("/boot/boot1.efifat");
		else
			return ("/boot/gptboot");
	}
	
	/* No partcode except for GPT */
	return (NULL);
}

