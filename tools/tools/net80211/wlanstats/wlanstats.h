/*-
 * Copyright (c) 2002-2007 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
 *    redistribution must be conditioned upon including a substantially
 *    similar Disclaimer requirement for further binary redistribution.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.
 *
 * $FreeBSD: release/10.1.0/tools/tools/net80211/wlanstats/wlanstats.h 174244 2007-12-04 05:52:01Z sam $
 */

#ifndef _WLANSTATS_H_
#define	_WLANSTATS_H_

#include "statfoo.h"

/*
 * wlan statistics class.
 */
struct wlanstatfoo {
	struct statfoo base;

	STATFOO_DECL_METHODS(struct wlanstatfoo *);

	/* set the network interface name for collection */
	void (*setifname)(struct wlanstatfoo *, const char *ifname);
	/* get the network interface name */
	const char *(*getifname)(struct wlanstatfoo *);
	/* get the wireless operating mode */
	int (*getopmode)(struct wlanstatfoo *);
	/* set the mac address of the associated station/ap */
	void (*setstamac)(struct wlanstatfoo *, const uint8_t mac[]);
};

struct wlanstatfoo *wlanstats_new(const char *ifname, const char *fmtstring);
#endif /* _WLANSTATS_H_ */
