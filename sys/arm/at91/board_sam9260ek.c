/*-
 * Copyright (c) 2012 Marius Strobl <marius@FreeBSD.org>
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
 */

/*
 * Ethernut 5 board support
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/sys/arm/at91/board_sam9260ek.c 266097 2014-05-14 23:51:07Z ian $");

#include <sys/param.h>
#include <sys/systm.h>
#include <machine/board.h>
#include <arm/at91/at91_pioreg.h>
#include <arm/at91/at91_piovar.h>
#include <arm/at91/at91board.h>
#include <arm/at91/at91sam9260reg.h>
#include <arm/at91/at91_smc.h>
#include <arm/at91/at91_gpio.h>
#include <dev/nand/nfc_at91.h>

static struct at91_smc_init nand_smc = {
	.ncs_rd_setup		= 0,
	.nrd_setup		= 1,
	.ncs_wr_setup		= 0,
	.nwe_setup		= 1,

	.ncs_rd_pulse		= 3,
	.nrd_pulse		= 3,
	.ncs_wr_pulse		= 3,
	.nwe_pulse		= 3,

	.nrd_cycle		= 5,
	.nwe_cycle		= 5,

	.mode			= SMC_MODE_READ | SMC_MODE_WRITE | SMC_MODE_EXNW_DISABLED,
	.tdf_cycles		= 2,
};

static struct at91_nand_params nand_param = {
	.ale			= 1u << 21,
	.cle			= 1u << 22,
	.width			= 8,
	.rnb_pin		= AT91_PIN_PC13,
	.nce_pin		= AT91_PIN_PC14,
	.cs			= 3,
};

static void
bi_dbgu(void)
{

	/*
	 * DBGU
	 */
	/* DRXD */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB14, 0);
	/* DTXD */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB15, 1);
}

static void
bi_emac(void)
{

	/*
	 * EMAC
	 */
	/* ETX0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA12, 0);
	/* ETX1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA13, 0);
	/* ERX0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA14, 0);
	/* ERX1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA15, 0);
	/* ETXEN */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA16, 0);
	/* ERXDV */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA17, 0);
	/* ERXER */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA18, 0);
	/* ETXCK */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA19, 0);
	/* EMDC */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA20, 0);
	/* EMDIO */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA21, 0);
	/* Not RMII */
	/* ETX2 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA10, 0);
	/* ETX3 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA11, 0);
	/* ETXER */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA22, 0);
	/* ERX2 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA25, 0);
	/* ERX3 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA26, 0);
	/* ERXCK */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA27, 0);
	/* ECRS */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA28, 0);
	/* ECOL */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA29, 0);
}

static void
bi_mmc(void)
{

	/*
	 * MMC, wired to socket B.
	 */
	/* MCDB0 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA0, 1);
	/* MCCDB */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA1, 1);
	/* MCDB3 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA3, 1);
	/* MCDB2 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA4, 1);
	/* MCDB1 */
	at91_pio_use_periph_b(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA5, 1);
	/* MCCK */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA8, 1);

	/*
	 * SPI0 and MMC are wired together, since we don't support sharing
	 * don't support the dataflash.  But if you did, you'd have to
	 * use CS0 and CS1.
	 */
}

static void
bi_iic(void)
{

	/*
	 * TWI.  Only one child on the iic bus, which we take care of
	 * via hints.
	 */
	/* TWD */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA23, 1);
	/* TWCK */
	at91_pio_use_periph_a(AT91SAM9260_PIOA_BASE, AT91C_PIO_PA24, 1);
}

static void
bi_usart0(void)
{

	/*
	 * USART0
	 */
	/* TXD0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB4, 1);
	/* RXD0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB5, 0);
	/* DSR0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB22, 0);
	/* DCD0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB23, 0);
	/* DTR0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB24, 1);
	/* RI0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB25, 0);
	/* RTS0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB26, 1);
	/* CTS0 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB27, 0);
}

static void
bi_usart1(void)
{
	/*
	 * USART1
	 */
	/* RTS1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB28, 1);
	/* CTS1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB29, 0);
	/* TXD1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB6, 1);
	/* RXD1 */
	at91_pio_use_periph_a(AT91SAM9260_PIOB_BASE, AT91C_PIO_PB7, 0);
}

static void
bi_nand(void)
{
	/* Samsung 256MB SLC Flash */

	/* Setup Static Memory Controller */
	at91_smc_setup(0, 3, &nand_smc);
	at91_enable_nand(&nand_param);

	/*
	 * This assumes
	 *  - RNB is on pin PC13
	 *  - CE is on pin PC14
	 *
	 * Nothing actually uses RNB right now.
	 *
	 * For CE, this currently asserts it during board setup and leaves it
	 * that way forever.
	 *
	 * All this can go away when the gpio pin-renumbering happens...
	 */
	at91_pio_use_gpio(AT91SAM9260_PIOC_BASE, AT91C_PIO_PC13 | AT91C_PIO_PC14);
	at91_pio_gpio_input(AT91SAM9260_PIOC_BASE, AT91C_PIO_PC13);	/* RNB */
	at91_pio_gpio_output(AT91SAM9260_PIOC_BASE, AT91C_PIO_PC14, 0);	/* nCS */
	at91_pio_gpio_clear(AT91SAM9260_PIOC_BASE, AT91C_PIO_PC14);	/* Assert nCS */
}

BOARD_INIT long
board_init(void)
{
	bi_dbgu();
	bi_emac();
	bi_mmc();

	/*
	 * SPI1 is wired to a audio CODEC that we don't support, so
	 * give it a pass.
	 */

	bi_iic();
	bi_usart0();
	bi_usart1();
	/* USART2 - USART5 aren't wired up, except via PIO pins, ignore them. */

	bi_nand();

	return (at91_ramsize());
}

ARM_BOARD(AT91SAM9260EK, "Atmel SMA9260-EK")
