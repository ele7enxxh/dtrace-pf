# $FreeBSD: release/10.1.0/usr.sbin/wpa/wpa_supplicant/Makefile 252726 2013-07-04 21:12:58Z rpaulo $

.include "${.CURDIR}/../Makefile.inc"

.PATH.c:${WPA_SUPPLICANT_DISTDIR} \
	${WPA_DISTDIR}/src/drivers

PROG=	wpa_supplicant
SRCS=	aes-unwrap.c base64.c blacklist.c bss.c common.c config.c \
	config_file.c ctrl_iface.c ctrl_iface_unix.c driver_bsd.c \
	driver_common.c driver_ndis.c driver_wired.c drivers.c \
	eap_register.c eloop.c events.c gas.c gas_query.c hs20.c \
	hs20_supplicant.c http_client.c http_server.c httpread.c \
	ieee802_11_common.c interworking.c l2_packet_freebsd.c main.c \
	md5.c notify.c offchannel.c os_unix.c peerkey.c pmksa_cache.c \
	preauth.c scan.c upnp_xml.c uuid.c wpa.c wpa_common.c wpa_debug.c \
	wpa_ft.c wpa_ie.c wpa_supplicant.c wpabuf.c wpas_glue.c wps.c \
	wps_attr_build.c wps_attr_parse.c wps_attr_process.c \
	wps_common.c wps_dev_attr.c wps_enrollee.c wps_registrar.c \
	wps_supplicant.c wps_upnp.c wps_upnp_ap.c wps_upnp_event.c \
	wps_upnp_ssdp.c wps_upnp_web.c Packet32.c

MAN=	wpa_supplicant.8 wpa_supplicant.conf.5

.if ${MK_EXAMPLES} != "no"
FILESDIR= ${SHAREDIR}/examples/etc
.PATH:	${WPA_SUPPLICANT_DISTDIR}
FILES=	wpa_supplicant.conf
.endif

CFLAGS+=-DCONFIG_BACKEND_FILE \
	-DCONFIG_DEBUG_SYSLOG \
	-DCONFIG_DRIVER_BSD \
	-DCONFIG_DRIVER_NDIS \
	-DCONFIG_DRIVER_WIRED \
	-DCONFIG_PEERKEY \
	-DCONFIG_SMARTCARD \
	-DCONFIG_TERMINATE_ONLASTIF \
	-DCONFIG_WPS \
	-DCONFIG_WPS2 \
	-DCONFIG_WPS_UPNP \
	-DCONFIG_TLS=openssl \
	-DCONFIG_IEEE80211R \
	-DCONFIG_INTERWORKING \
	-DCONFIG_PRIVSEP \
	-DCONFIG_HS20 \
	-DCONFIG_GAS \
	-DPKCS12_FUNCS
#CFLAGS+= -g
DPADD+=	${LIBPCAP}
LDADD+=	-lpcap

# User customizations to the wpa_supplicant build environment
CFLAGS+=${WPA_SUPPLICANT_CFLAGS}
#DPADD+=${WPA_SUPPLICANT_DPADD}
LDADD+=${WPA_SUPPLICANT_LDADD}
#LDFLAGS+=${WPA_SUPPLICANT_LDFLAGS}

.if ${MK_WPA_SUPPLICANT_EAPOL} != "no"
CFLAGS+=-DEAP_GTC \
	-DEAP_LEAP \
	-DEAP_MD5 \
	-DEAP_MSCHAPv2 \
	-DEAP_OTP \
	-DEAP_PEAP \
	-DEAP_PSK \
	-DEAP_TLS \
	-DEAP_TTLS \
	-DEAP_GTC \
	-DEAP_OTP \
	-DEAP_LEAP \
	-DIEEE8021X_EAPOL
SRCS+=	chap.c \
	eap.c \
	eap_common.c \
	eap_gtc.c \
	eap_leap.c \
	eap_md5.c \
	eap_methods.c \
	eap_mschapv2.c \
	eap_otp.c \
	eap_peap.c \
	eap_peap_common.c \
	eap_psk.c \
	eap_psk_common.c \
	eap_tls.c \
	eap_tls_common.c \
	eap_ttls.c \
	eapol_supp_sm.c \
	ms_funcs.c \
	mschapv2.c
TLS_FUNCS=y
NEED_AES_EAX=y
NEED_AES_ENCBLOCK=y
NEED_AES_OMAC1=y
.endif

.if !empty(CFLAGS:M-DCONFIG_WPS)
NEED_AES_CBC=y
.endif

.if !empty(CFLAGS:M*-DEAP_AKA)
SRCS+=	eap_aka.c
NEED_SIM_COMMON=y
NEED_AES_CBC=y
.endif

.if !empty(CFLAGS:M*-DEAP_SIM)
SRCS+=	eap_sim.c
NEED_SIM_COMMON=y
NEED_AES_CBC=y
.endif

.if defined(NEED_SIM_COMMON)
SRCS+=	eap_sim_common.c
NEED_FIPS186_2_PRF=y
.endif

# PC/SC interface for smartcards (USIM, GSM SIM)
# GSM/UMTS authentication algorithm (for EAP-SIM/EAP-AKA)
# NB: requires devel/pcsc-lite
#
# WPA_SUPPLICANT_CFLAGS=-DEAP_AKA -DPCSC_FUNCS -I/usr/local/include/PCSC
# WPA_SUPPLICANT_LDADD=-L/usr/local/lib
#
.if !empty(CFLAGS:M*-DPCSC_FUNCS)
SRCS+=	pcsc_funcs.c
DPADD+=${LIBPTHREAD}
LDADD+=-lpcsclite -lpthread
.endif

.if !empty(CFLAGS:M*-DEAP_GPSK)
CFLAGS+=-DEAP_GPSK_SHA256
SRCS+=	eap_gpsk.c \
	eap_gpsk_common.c
NEED_AES_OMAC1=y
.endif

.if !empty(CFLAGS:M*-DEAP_PAX)
SRCS+=	eap_pax.c \
	eap_pax_common.c
.endif

.if !empty(CFLAGS:M*-DEAP_SAKE)
SRCS+=	eap_sake.c \
	eap_sake_common.c
.endif

.include "${.CURDIR}/../Makefile.crypto"

.include <bsd.prog.mk>
