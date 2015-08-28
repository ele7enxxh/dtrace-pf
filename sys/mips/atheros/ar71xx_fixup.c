/*-
 * Copyright (c) 2009, Oleksandr Tymoshenko <gonzo@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
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
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/sys/mips/atheros/ar71xx_fixup.c 234485 2012-04-20 08:26:05Z adrian $");

#include "opt_ar71xx.h"

#include <sys/param.h>
#include <sys/systm.h>

#include <sys/bus.h>
#include <sys/interrupt.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/rman.h>
#include <sys/lock.h>
#include <sys/mutex.h>

#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_extern.h>

#include <machine/bus.h>
#include <machine/cpu.h>
#include <machine/intr_machdep.h>
#include <machine/pmap.h>

#include <dev/pci/pcivar.h>
#include <dev/pci/pcireg.h>

#include <dev/pci/pcib_private.h>
#include "pcib_if.h"

#include <mips/atheros/ar71xxreg.h>
#include <mips/atheros/ar71xx_pci_bus_space.h>

#include <mips/atheros/ar71xx_cpudef.h>

#include <sys/linker.h>
#include <sys/firmware.h>

#include <mips/atheros/ar71xx_fixup.h>

/*
 * Take a copy of the EEPROM contents and squirrel it away in a firmware.
 * The SPI flash will eventually cease to be memory-mapped, so we need
 * to take a copy of this before the SPI driver initialises.
 */
void
ar71xx_pci_slot_create_eeprom_firmware(device_t dev, u_int bus, u_int slot,
    u_int func, long int flash_addr, int size)
{
	char buf[64];
	uint16_t *cal_data = (uint16_t *) MIPS_PHYS_TO_KSEG1(flash_addr);
	void *eeprom = NULL;
	const struct firmware *fw = NULL;

	device_printf(dev, "EEPROM firmware: 0x%lx @ %d bytes\n",
	    flash_addr, size);

	eeprom = malloc(size, M_DEVBUF, M_WAITOK | M_ZERO);
	if (! eeprom) {
		device_printf(dev,
			    "%s: malloc failed for '%s', aborting EEPROM\n",
			    __func__, buf);
			return;
	}

	memcpy(eeprom, cal_data, size);

	/*
	 * Generate a flash EEPROM 'firmware' from the given memory
	 * region.  Since the SPI controller will eventually
	 * go into port-IO mode instead of memory-mapped IO
	 * mode, a copy of the EEPROM contents is required.
	 */
	snprintf(buf, sizeof(buf), "%s.%d.bus.%d.%d.%d.eeprom_firmware",
	    device_get_name(dev), device_get_unit(dev), bus, slot, func);
	fw = firmware_register(buf, eeprom, size, 1, NULL);
	if (fw == NULL) {
		device_printf(dev, "%s: firmware_register (%s) failed\n",
		    __func__, buf);
		free(eeprom, M_DEVBUF);
		return;
	}
	device_printf(dev, "device EEPROM '%s' registered\n", buf);
}

#if 0
static void
ar71xx_pci_slot_fixup(device_t dev, u_int bus, u_int slot, u_int func)
{
	long int flash_addr;
	char buf[64];
	int size;

	/*
	 * Check whether the given slot has a hint to poke.
	 */
	if (bootverbose)
	device_printf(dev, "%s: checking dev %s, %d/%d/%d\n",
	    __func__, device_get_nameunit(dev), bus, slot, func);

	snprintf(buf, sizeof(buf), "bus.%d.%d.%d.ath_fixup_addr",
	    bus, slot, func);

	if (resource_long_value(device_get_name(dev), device_get_unit(dev),
	    buf, &flash_addr) == 0) {
		snprintf(buf, sizeof(buf), "bus.%d.%d.%d.ath_fixup_size",
		    bus, slot, func);
		if (resource_int_value(device_get_name(dev),
		    device_get_unit(dev), buf, &size) != 0) {
			device_printf(dev,
			    "%s: missing hint '%s', aborting EEPROM\n",
			    __func__, buf);
			return;
		}


		device_printf(dev, "found EEPROM at 0x%lx on %d.%d.%d\n",
		    flash_addr, bus, slot, func);
		ar71xx_pci_fixup(dev, bus, slot, func, flash_addr, size);
		ar71xx_pci_slot_create_eeprom_firmware(dev, bus, slot, func,
		    flash_addr, size);
	}
}
#endif /* 0 */
