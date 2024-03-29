/*-
 * Copyright (c) 2009, Oleksandr Tymoshenko <gonzo@FreeBSD.org>
 * Copyright (c) 2009, Luiz Otavio O Souza. 
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

/*
 * GPIO driver for AR71xx 
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/sys/mips/atheros/ar71xx_gpio.c 255335 2013-09-06 23:47:50Z loos $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>

#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/rman.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/gpio.h>

#include <machine/bus.h>
#include <machine/resource.h>
#include <mips/atheros/ar71xxreg.h>
#include <mips/atheros/ar71xx_setup.h>
#include <mips/atheros/ar71xx_gpiovar.h>
#include <mips/atheros/ar933xreg.h>
#include <mips/atheros/ar934xreg.h>

#include "gpio_if.h"

#define	DEFAULT_CAPS	(GPIO_PIN_INPUT | GPIO_PIN_OUTPUT)

/*
 * Helpers
 */
static void ar71xx_gpio_function_enable(struct ar71xx_gpio_softc *sc, 
    uint32_t mask);
static void ar71xx_gpio_function_disable(struct ar71xx_gpio_softc *sc, 
    uint32_t mask);
static void ar71xx_gpio_pin_configure(struct ar71xx_gpio_softc *sc, 
    struct gpio_pin *pin, uint32_t flags);

/*
 * Driver stuff
 */
static int ar71xx_gpio_probe(device_t dev);
static int ar71xx_gpio_attach(device_t dev);
static int ar71xx_gpio_detach(device_t dev);
static int ar71xx_gpio_filter(void *arg);
static void ar71xx_gpio_intr(void *arg);

/*
 * GPIO interface
 */
static int ar71xx_gpio_pin_max(device_t dev, int *maxpin);
static int ar71xx_gpio_pin_getcaps(device_t dev, uint32_t pin, uint32_t *caps);
static int ar71xx_gpio_pin_getflags(device_t dev, uint32_t pin, uint32_t
    *flags);
static int ar71xx_gpio_pin_getname(device_t dev, uint32_t pin, char *name);
static int ar71xx_gpio_pin_setflags(device_t dev, uint32_t pin, uint32_t flags);
static int ar71xx_gpio_pin_set(device_t dev, uint32_t pin, unsigned int value);
static int ar71xx_gpio_pin_get(device_t dev, uint32_t pin, unsigned int *val);
static int ar71xx_gpio_pin_toggle(device_t dev, uint32_t pin);

static void
ar71xx_gpio_function_enable(struct ar71xx_gpio_softc *sc, uint32_t mask)
{
	if (ar71xx_soc == AR71XX_SOC_AR9341 ||
	    ar71xx_soc == AR71XX_SOC_AR9342 ||
	    ar71xx_soc == AR71XX_SOC_AR9344)
		GPIO_SET_BITS(sc, AR934X_GPIO_REG_FUNC, mask);
	else
		GPIO_SET_BITS(sc, AR71XX_GPIO_FUNCTION, mask);
}

static void
ar71xx_gpio_function_disable(struct ar71xx_gpio_softc *sc, uint32_t mask)
{
	if (ar71xx_soc == AR71XX_SOC_AR9341 ||
	    ar71xx_soc == AR71XX_SOC_AR9342 ||
	    ar71xx_soc == AR71XX_SOC_AR9344)
		GPIO_CLEAR_BITS(sc, AR934X_GPIO_REG_FUNC, mask);
	else
		GPIO_CLEAR_BITS(sc, AR71XX_GPIO_FUNCTION, mask);
}

static void
ar71xx_gpio_pin_configure(struct ar71xx_gpio_softc *sc, struct gpio_pin *pin,
    unsigned int flags)
{
	uint32_t mask;

	mask = 1 << pin->gp_pin;

	/*
	 * Manage input/output
	 */
	if (flags & (GPIO_PIN_INPUT|GPIO_PIN_OUTPUT)) {
		pin->gp_flags &= ~(GPIO_PIN_INPUT|GPIO_PIN_OUTPUT);
		if (flags & GPIO_PIN_OUTPUT) {
			pin->gp_flags |= GPIO_PIN_OUTPUT;
			GPIO_SET_BITS(sc, AR71XX_GPIO_OE, mask);
		}
		else {
			pin->gp_flags |= GPIO_PIN_INPUT;
			GPIO_CLEAR_BITS(sc, AR71XX_GPIO_OE, mask);
		}
	}
}

static int
ar71xx_gpio_pin_max(device_t dev, int *maxpin)
{

	switch (ar71xx_soc) {
		case AR71XX_SOC_AR9130:
		case AR71XX_SOC_AR9132:
			*maxpin = AR91XX_GPIO_PINS - 1;
			break;
		case AR71XX_SOC_AR7240:
		case AR71XX_SOC_AR7241:
		case AR71XX_SOC_AR7242:
			*maxpin = AR724X_GPIO_PINS - 1;
			break;
		case AR71XX_SOC_AR9330:
		case AR71XX_SOC_AR9331:
			*maxpin = AR933X_GPIO_COUNT - 1;
			break;
		case AR71XX_SOC_AR9341:
		case AR71XX_SOC_AR9342:
		case AR71XX_SOC_AR9344:
			*maxpin = AR934X_GPIO_COUNT - 1;
			break;
		default:
			*maxpin = AR71XX_GPIO_PINS - 1;
	}
	return (0);
}

static int
ar71xx_gpio_pin_getcaps(device_t dev, uint32_t pin, uint32_t *caps)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int i;

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	GPIO_LOCK(sc);
	*caps = sc->gpio_pins[i].gp_caps;
	GPIO_UNLOCK(sc);

	return (0);
}

static int
ar71xx_gpio_pin_getflags(device_t dev, uint32_t pin, uint32_t *flags)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int i;

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	GPIO_LOCK(sc);
	*flags = sc->gpio_pins[i].gp_flags;
	GPIO_UNLOCK(sc);

	return (0);
}

static int
ar71xx_gpio_pin_getname(device_t dev, uint32_t pin, char *name)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int i;

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	GPIO_LOCK(sc);
	memcpy(name, sc->gpio_pins[i].gp_name, GPIOMAXNAME);
	GPIO_UNLOCK(sc);

	return (0);
}

static int
ar71xx_gpio_pin_setflags(device_t dev, uint32_t pin, uint32_t flags)
{
	int i;
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	/* Check for unwanted flags. */
	if ((flags & sc->gpio_pins[i].gp_caps) != flags)
		return (EINVAL);

	/* Can't mix input/output together */
	if ((flags & (GPIO_PIN_INPUT|GPIO_PIN_OUTPUT)) ==
	    (GPIO_PIN_INPUT|GPIO_PIN_OUTPUT))
		return (EINVAL);

	ar71xx_gpio_pin_configure(sc, &sc->gpio_pins[i], flags);
	return (0);
}

static int
ar71xx_gpio_pin_set(device_t dev, uint32_t pin, unsigned int value)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int i;

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	if (value)
		GPIO_WRITE(sc, AR71XX_GPIO_SET, (1 << pin));
	else
		GPIO_WRITE(sc, AR71XX_GPIO_CLEAR, (1 << pin));

	return (0);
}

static int
ar71xx_gpio_pin_get(device_t dev, uint32_t pin, unsigned int *val)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int i;

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	*val = (GPIO_READ(sc, AR71XX_GPIO_IN) & (1 << pin)) ? 1 : 0;

	return (0);
}

static int
ar71xx_gpio_pin_toggle(device_t dev, uint32_t pin)
{
	int res, i;
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);

	for (i = 0; i < sc->gpio_npins; i++) {
		if (sc->gpio_pins[i].gp_pin == pin)
			break;
	}

	if (i >= sc->gpio_npins)
		return (EINVAL);

	res = (GPIO_READ(sc, AR71XX_GPIO_IN) & (1 << pin)) ? 1 : 0;
	if (res)
		GPIO_WRITE(sc, AR71XX_GPIO_CLEAR, (1 << pin));
	else
		GPIO_WRITE(sc, AR71XX_GPIO_SET, (1 << pin));

	return (0);
}

static int
ar71xx_gpio_filter(void *arg)
{

	/* TODO: something useful */
	return (FILTER_STRAY);
}



static void
ar71xx_gpio_intr(void *arg)
{
	struct ar71xx_gpio_softc *sc = arg;
	GPIO_LOCK(sc);
	/* TODO: something useful */
	GPIO_UNLOCK(sc);
}

static int
ar71xx_gpio_probe(device_t dev)
{

	device_set_desc(dev, "Atheros AR71XX GPIO driver");
	return (0);
}

static int
ar71xx_gpio_attach(device_t dev)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);
	int error = 0;
	int i, j, maxpin;
	int mask, pinon;
	int old = 0;

	KASSERT((device_get_unit(dev) == 0),
	    ("ar71xx_gpio: Only one gpio module supported"));

	mtx_init(&sc->gpio_mtx, device_get_nameunit(dev), NULL, MTX_DEF);

	/* Map control/status registers. */
	sc->gpio_mem_rid = 0;
	sc->gpio_mem_res = bus_alloc_resource_any(dev, SYS_RES_MEMORY,
	    &sc->gpio_mem_rid, RF_ACTIVE);

	if (sc->gpio_mem_res == NULL) {
		device_printf(dev, "couldn't map memory\n");
		error = ENXIO;
		ar71xx_gpio_detach(dev);
		return(error);
	}

	if ((sc->gpio_irq_res = bus_alloc_resource_any(dev, SYS_RES_IRQ, 
	    &sc->gpio_irq_rid, RF_SHAREABLE | RF_ACTIVE)) == NULL) {
		device_printf(dev, "unable to allocate IRQ resource\n");
		return (ENXIO);
	}

	if ((bus_setup_intr(dev, sc->gpio_irq_res, INTR_TYPE_MISC, 
	    ar71xx_gpio_filter, ar71xx_gpio_intr, sc, &sc->gpio_ih))) {
		device_printf(dev,
		    "WARNING: unable to register interrupt handler\n");
		return (ENXIO);
	}

	sc->dev = dev;

	/* Enable function bits that are required */
	if (resource_int_value(device_get_name(dev), device_get_unit(dev),
	    "function_set", &mask) == 0) {
		device_printf(dev, "function_set: 0x%x\n", mask);
		ar71xx_gpio_function_enable(sc, mask);
		old = 1;
	}
	/* Disable function bits that are required */
	if (resource_int_value(device_get_name(dev), device_get_unit(dev),
	    "function_clear", &mask) == 0) {
		device_printf(dev, "function_clear: 0x%x\n", mask);
		ar71xx_gpio_function_disable(sc, mask);
		old = 1;
	}
	/* Handle previous behaviour */
	if (old == 0) {
		ar71xx_gpio_function_enable(sc, GPIO_FUNC_SPI_CS1_EN);
		ar71xx_gpio_function_enable(sc, GPIO_FUNC_SPI_CS2_EN);
	}

	/* Configure all pins as input */
	/* disable interrupts for all pins */
	GPIO_WRITE(sc, AR71XX_GPIO_INT_MASK, 0);

	/* Initialise all pins specified in the mask, up to the pin count */
	(void) ar71xx_gpio_pin_max(dev, &maxpin);
	if (resource_int_value(device_get_name(dev), device_get_unit(dev),
	    "pinmask", &mask) != 0)
		mask = 0;
	if (resource_int_value(device_get_name(dev), device_get_unit(dev),
	    "pinon", &pinon) != 0)
		pinon = 0;
	device_printf(dev, "gpio pinmask=0x%x\n", mask);
	for (j = 0; j <= maxpin; j++) {
		if ((mask & (1 << j)) == 0)
			continue;
		sc->gpio_npins++;
	}
	sc->gpio_pins = malloc(sizeof(*sc->gpio_pins) * sc->gpio_npins,
	    M_DEVBUF, M_WAITOK | M_ZERO);
	for (i = 0, j = 0; j <= maxpin; j++) {
		if ((mask & (1 << j)) == 0)
			continue;
		snprintf(sc->gpio_pins[i].gp_name, GPIOMAXNAME,
		    "pin %d", j);
		sc->gpio_pins[i].gp_pin = j;
		sc->gpio_pins[i].gp_caps = DEFAULT_CAPS;
		sc->gpio_pins[i].gp_flags = 0;
		ar71xx_gpio_pin_configure(sc, &sc->gpio_pins[i], DEFAULT_CAPS);
		i++;
	}
	for (i = 0; i < sc->gpio_npins; i++) {
		j = sc->gpio_pins[i].gp_pin;
		if ((pinon & (1 << j)) != 0)
			ar71xx_gpio_pin_set(dev, j, 1);
	}
	device_add_child(dev, "gpioc", device_get_unit(dev));
	device_add_child(dev, "gpiobus", device_get_unit(dev));
	return (bus_generic_attach(dev));
}

static int
ar71xx_gpio_detach(device_t dev)
{
	struct ar71xx_gpio_softc *sc = device_get_softc(dev);

	KASSERT(mtx_initialized(&sc->gpio_mtx), ("gpio mutex not initialized"));

	ar71xx_gpio_function_disable(sc, GPIO_FUNC_SPI_CS1_EN);
	ar71xx_gpio_function_disable(sc, GPIO_FUNC_SPI_CS2_EN);
	bus_generic_detach(dev);

	if (sc->gpio_mem_res)
		bus_release_resource(dev, SYS_RES_MEMORY, sc->gpio_mem_rid,
		    sc->gpio_mem_res);

	free(sc->gpio_pins, M_DEVBUF);
	mtx_destroy(&sc->gpio_mtx);

	return(0);
}

static device_method_t ar71xx_gpio_methods[] = {
	DEVMETHOD(device_probe, ar71xx_gpio_probe),
	DEVMETHOD(device_attach, ar71xx_gpio_attach),
	DEVMETHOD(device_detach, ar71xx_gpio_detach),

	/* GPIO protocol */
	DEVMETHOD(gpio_pin_max, ar71xx_gpio_pin_max),
	DEVMETHOD(gpio_pin_getname, ar71xx_gpio_pin_getname),
	DEVMETHOD(gpio_pin_getflags, ar71xx_gpio_pin_getflags),
	DEVMETHOD(gpio_pin_getcaps, ar71xx_gpio_pin_getcaps),
	DEVMETHOD(gpio_pin_setflags, ar71xx_gpio_pin_setflags),
	DEVMETHOD(gpio_pin_get, ar71xx_gpio_pin_get),
	DEVMETHOD(gpio_pin_set, ar71xx_gpio_pin_set),
	DEVMETHOD(gpio_pin_toggle, ar71xx_gpio_pin_toggle),
	{0, 0},
};

static driver_t ar71xx_gpio_driver = {
	"gpio",
	ar71xx_gpio_methods,
	sizeof(struct ar71xx_gpio_softc),
};
static devclass_t ar71xx_gpio_devclass;

DRIVER_MODULE(ar71xx_gpio, apb, ar71xx_gpio_driver, ar71xx_gpio_devclass, 0, 0);
