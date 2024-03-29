/*-
 * Copyright (c) 2012 Oleksandr Tymoshenko <gonzo@freebsd.org>
 * Copyright (c) 2013 Luiz Otavio O Souza <loos@freebsd.org>
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
 * $FreeBSD: release/10.1.0/sys/arm/broadcom/bcm2835/bcm2835_bscvar.h 261078 2014-01-23 12:32:30Z loos $
 */

#ifndef _BCM2835_BSCVAR_H
#define _BCM2835_BSCVAR_H

struct {
	uint32_t	sda;
	uint32_t	scl;
	unsigned long	start;
} bcm_bsc_pins[] = {
	{ 0, 1, 0x20205000 },	/* BSC0 GPIO pins and base address. */
	{ 2, 3, 0x20804000 }	/* BSC1 GPIO pins and base address. */
};

struct bcm_bsc_softc {
	device_t		sc_dev;
	struct mtx		sc_mtx;
	struct resource *	sc_mem_res;
	struct resource *	sc_irq_res;
	bus_space_tag_t		sc_bst;
	bus_space_handle_t	sc_bsh;
	uint16_t		sc_resid;
	uint8_t			*sc_data;
	uint8_t			sc_flags;
	void *			sc_intrhand;
};

#define	BCM_I2C_BUSY		0x01
#define	BCM_I2C_READ		0x02
#define	BCM_I2C_ERROR		0x04

#define	BCM_BSC_WRITE(_sc, _off, _val)		\
    bus_space_write_4(_sc->sc_bst, _sc->sc_bsh, _off, _val)
#define	BCM_BSC_READ(_sc, _off)			\
    bus_space_read_4(_sc->sc_bst, _sc->sc_bsh, _off)

#define	BCM_BSC_LOCK(_sc)			\
    mtx_lock(&(_sc)->sc_mtx)
#define	BCM_BSC_UNLOCK(_sc)			\
    mtx_unlock(&(_sc)->sc_mtx)

#endif	/* _BCM2835_BSCVAR_H_ */
