/*
 * $FreeBSD: release/10.1.0/sys/dev/etherswitch/etherswitch.h 253569 2013-07-23 13:56:38Z loos $
 */

#ifndef __SYS_DEV_ETHERSWITCH_ETHERSWITCH_H
#define __SYS_DEV_ETHERSWITCH_ETHERSWITCH_H

#include <sys/ioccom.h>

#ifdef _KERNEL
extern devclass_t       etherswitch_devclass;
extern driver_t         etherswitch_driver;
#endif /* _KERNEL */

struct etherswitch_reg {
	uint16_t	reg;
	uint16_t	val;
};
typedef struct etherswitch_reg etherswitch_reg_t;

struct etherswitch_phyreg {
	uint16_t	phy;
	uint16_t	reg;
	uint16_t	val;
};
typedef struct etherswitch_phyreg etherswitch_phyreg_t;

#define	ETHERSWITCH_NAMEMAX		64
#define	ETHERSWITCH_VID_MASK		0xfff
#define	ETHERSWITCH_VID_VALID		(1 << 12)
#define	ETHERSWITCH_VLAN_ISL		(1 << 0)	/* ISL */
#define	ETHERSWITCH_VLAN_PORT		(1 << 1)	/* Port based vlan */
#define	ETHERSWITCH_VLAN_DOT1Q		(1 << 2)	/* 802.1q */
#define	ETHERSWITCH_VLAN_DOT1Q_4K	(1 << 3)	/* 4k support on 802.1q */
#define	ETHERSWITCH_VLAN_DOUBLE_TAG	(1 << 4)	/* Q-in-Q */
#define	ETHERSWITCH_VLAN_CAPS_BITS	\
"\020\1ISL\2PORT\3DOT1Q\4DOT1Q4K\5QinQ"

struct etherswitch_info {
	int		es_nports;
	int		es_nvlangroups;
	char		es_name[ETHERSWITCH_NAMEMAX];
	uint32_t	es_vlan_caps;
};
typedef struct etherswitch_info etherswitch_info_t;

#define	ETHERSWITCH_CONF_FLAGS		(1 << 0)
#define	ETHERSWITCH_CONF_MIRROR		(1 << 1)
#define	ETHERSWITCH_CONF_VLAN_MODE	(1 << 2)

struct etherswitch_conf {
	uint32_t	cmd;		/* What to configure */
	uint32_t	vlan_mode;	/* Switch VLAN mode */
};
typedef struct etherswitch_conf etherswitch_conf_t;

#define	ETHERSWITCH_PORT_CPU		(1 << 0)
#define	ETHERSWITCH_PORT_STRIPTAG	(1 << 1)
#define	ETHERSWITCH_PORT_ADDTAG		(1 << 2)
#define	ETHERSWITCH_PORT_FIRSTLOCK	(1 << 3)
#define	ETHERSWITCH_PORT_DROPUNTAGGED	(1 << 4)
#define	ETHERSWITCH_PORT_DOUBLE_TAG	(1 << 5)
#define	ETHERSWITCH_PORT_INGRESS	(1 << 6)
#define	ETHERSWITCH_PORT_FLAGS_BITS	\
"\020\1CPUPORT\2STRIPTAG\3ADDTAG\4FIRSTLOCK\5DROPUNTAGGED\6QinQ\7INGRESS"

struct etherswitch_port {
	int		es_port;
	int		es_pvid;
	uint32_t	es_flags;
	union {
		struct ifreq		es_uifr;
		struct ifmediareq	es_uifmr;
	} es_ifu;
#define es_ifr		es_ifu.es_uifr
#define es_ifmr		es_ifu.es_uifmr
};
typedef struct etherswitch_port etherswitch_port_t;

struct etherswitch_vlangroup {
	int		es_vlangroup;
	int		es_vid;
	int		es_member_ports;
	int		es_untagged_ports;
	int		es_fid;
};
typedef struct etherswitch_vlangroup etherswitch_vlangroup_t;

#define ETHERSWITCH_PORTMASK(_port)	(1 << (_port))

#define IOETHERSWITCHGETINFO		_IOR('i', 1, etherswitch_info_t)
#define IOETHERSWITCHGETREG		_IOWR('i', 2, etherswitch_reg_t)
#define IOETHERSWITCHSETREG		_IOW('i', 3, etherswitch_reg_t)
#define IOETHERSWITCHGETPORT		_IOWR('i', 4, etherswitch_port_t)
#define IOETHERSWITCHSETPORT		_IOW('i', 5, etherswitch_port_t)
#define IOETHERSWITCHGETVLANGROUP	_IOWR('i', 6, etherswitch_vlangroup_t)
#define IOETHERSWITCHSETVLANGROUP	_IOW('i', 7, etherswitch_vlangroup_t)
#define IOETHERSWITCHGETPHYREG		_IOWR('i', 8, etherswitch_phyreg_t)
#define IOETHERSWITCHSETPHYREG		_IOW('i', 9, etherswitch_phyreg_t)
#define IOETHERSWITCHGETCONF		_IOR('i', 10, etherswitch_conf_t)
#define IOETHERSWITCHSETCONF		_IOW('i', 11, etherswitch_conf_t)

#endif
