/*-
 * Copyright (c) 2007 Bruce M. Simpson.
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
 * $Id$
 */
/*
 * Skeleton of this file was based on respective code for ARM
 * code written by Olivier Houchard.
 */

/*
 * XXXMIPS: This file is hacked from arm/... . XXXMIPS here means this file is
 * experimental and was written for MIPS32 port.
 */
#include "opt_uart.h"

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/sys/mips/idt/uart_bus_rc32434.c 202175 2010-01-12 21:36:08Z imp $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <machine/bus.h>
#include <sys/rman.h>
#include <machine/resource.h>
#include <mips/idt/idtreg.h>

#include <dev/pci/pcivar.h>

#include <dev/uart/uart.h>
#include <dev/uart/uart_bus.h>
#include <dev/uart/uart_cpu.h>

#include <dev/ic/ns16550.h>

#include "uart_if.h"

static int uart_rc32434_probe(device_t dev);

extern struct uart_class uart_rc32434_uart_class;

static device_method_t uart_rc32434_methods[] = {
	/* Device interface */
	DEVMETHOD(device_probe,		uart_rc32434_probe),
	DEVMETHOD(device_attach,	uart_bus_attach),
	DEVMETHOD(device_detach,	uart_bus_detach),
	{ 0, 0 }
};

static driver_t uart_rc32434_driver = {
	uart_driver_name,
	uart_rc32434_methods,
	sizeof(struct uart_softc),
};

extern SLIST_HEAD(uart_devinfo_list, uart_devinfo) uart_sysdevs;

static int
uart_rc32434_probe(device_t dev)
{
	struct uart_softc *sc;

	sc = device_get_softc(dev);
	sc->sc_sysdev = SLIST_FIRST(&uart_sysdevs);
	sc->sc_class = &uart_ns8250_class;
	bcopy(&sc->sc_sysdev->bas, &sc->sc_bas, sizeof(sc->sc_bas));
	sc->sc_sysdev->bas.regshft = 2;
	sc->sc_sysdev->bas.bst = mips_bus_space_generic;
	sc->sc_sysdev->bas.bsh = MIPS_PHYS_TO_KSEG1(IDT_BASE_UART0);
	sc->sc_bas.regshft = 2;
	sc->sc_bas.bst = mips_bus_space_generic;
	sc->sc_bas.bsh = MIPS_PHYS_TO_KSEG1(IDT_BASE_UART0);

	return (uart_bus_probe(dev, 2, 330000000UL/2, 0, 0));
}

DRIVER_MODULE(uart, obio, uart_rc32434_driver, uart_devclass, 0, 0);
