/*-
 * Copyright (c) 2003-2009 Silicon Graphics International Corp.
 * Copyright (c) 2011 Spectra Logic Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce at minimum a disclaimer
 *    substantially similar to the "NO WARRANTY" disclaimer below
 *    ("Disclaimer") and any redistribution must be conditioned upon
 *    including a substantially similar Disclaimer requirement for further
 *    binary redistribution.
 *
 * NO WARRANTY
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * $Id: //depot/users/kenm/FreeBSD-test2/sys/cam/ctl/ctl_ha.h#1 $
 * $FreeBSD: release/10.1.0/sys/cam/ctl/ctl_ha.h 229997 2012-01-12 00:34:33Z ken $
 */

#ifndef _CTL_HA_H_
#define	_CTL_HA_H_

/*
 * CTL High Availability Modes:
 *
 * CTL_HA_MODE_SER_ONLY:  Commands are serialized to the other side.  Write
 *			  mirroring and read re-direction are assumed to
 * 			  happen in the back end.
 * CTL_HA_MODE_XFER:	  Commands are serialized and data is transferred
 *			  for write mirroring and read re-direction.
 */

typedef enum {
	CTL_HA_MODE_SER_ONLY,
	CTL_HA_MODE_XFER
} ctl_ha_mode;


/*
 * This is a stubbed out High Availability interface.  It assumes two nodes
 * staying in sync.
 *
 * The reason this interface is here, and stubbed out, is that CTL was
 * originally written with support for Copan's (now SGI) high availability
 * framework.  That framework was not released by SGI, and would not have
 * been generally applicable to FreeBSD anyway.
 *
 * The idea here is to show the kind of API that would need to be in place
 * in a HA framework to work with CTL's HA hooks.  This API is very close
 * to the Copan/SGI API, so that the code using it could stay in place
 * as-is.
 *
 * So, in summary, this is a shell without real substance, and much more
 * work would be needed to actually make HA work.  The implementation
 * inside CTL will also need to change to fit the eventual implementation.
 * The additional pieces we would need are:
 *
 *  - HA "Supervisor" framework that can startup the components of the
 *    system, and initiate failover (i.e. active/active to single mode)
 *    and failback (single to active/active mode) state transitions.
 *    This framework would be able to recognize when an event happens
 *    that requires it to initiate state transitions in the components it
 *    manages.
 *
 *  - HA communication framework.  This framework should have the following
 *    features:
 *	- Separate channels for separate system components.  The CTL
 *	  instance on one node should communicate with the CTL instance
 *	  on another node.
 *	- Short message passing.  These messages would be fixed length, so
 *	  they could be preallocated and easily passed between the nodes.
 *	  i.e. conceptually like an ethernet packet.
 *	- DMA/large buffer capability.  This would require some negotiation
 *	  with the other node to define the destination.  It could
 *	  allow for "push" (i.e. initiated by the requesting node) DMA or
 * 	  "pull" (i.e. initiated by the target controller) DMA or both.
 *	- Communication channel status change notification.
 *  - HA capability in other portions of the storage stack.  Having two CTL
 *    instances communicate is just one part of an overall HA solution.
 *    State needs to be synchronized at multiple levels of the system in
 *    order for failover to actually work.  For instance, if CTL is using a
 *    file on a ZFS filesystem as its backing store, the ZFS array state
 *    should be synchronized with the other node, so that the other node
 *    can immediately take over if the node that is primary for a particular
 *    array fails.
 */

/*
 * Communication channel IDs for various system components.  This is to
 * make sure one CTL instance talks with another, one ZFS instance talks
 * with another, etc.
 */
typedef enum {
	CTL_HA_CHAN_NONE,
	CTL_HA_CHAN_CTL,
	CTL_HA_CHAN_ZFS,
	CTL_HA_CHAN_MAX
} ctl_ha_channel;

/*
 * HA communication event notification.  These are events generated by the
 * HA communication subsystem.
 *
 * CTL_HA_EVT_MSG_RECV:		Message received by the other node.
 * CTL_HA_EVT_MSG_SENT:		Message sent to the other node.
 * CTL_HA_EVT_DISCONNECT:	Communication channel disconnected.
 * CTL_HA_EVT_DMA_SENT:		DMA successfully sent to other node (push).
 * CTL_HA_EVT_DMA_RECEIVED:	DMA successfully received by other node (pull).
 */
typedef enum {
	CTL_HA_EVT_NONE,
	CTL_HA_EVT_MSG_RECV,
	CTL_HA_EVT_MSG_SENT,
	CTL_HA_EVT_DISCONNECT,
	CTL_HA_EVT_DMA_SENT,
	CTL_HA_EVT_DMA_RECEIVED,
	CTL_HA_EVT_MAX
} ctl_ha_event;

typedef enum {
	CTL_HA_STATUS_WAIT,
	CTL_HA_STATUS_SUCCESS,
	CTL_HA_STATUS_ERROR,
	CTL_HA_STATUS_INVALID,
	CTL_HA_STATUS_DISCONNECT,
	CTL_HA_STATUS_BUSY,
	CTL_HA_STATUS_MAX
} ctl_ha_status;

typedef enum {
	CTL_HA_DATA_CTL,
	CTL_HA_DATA_ZFS,
	CTL_HA_DATA_MAX
} ctl_ha_dtid;

typedef enum {
	CTL_HA_DT_CMD_READ,
	CTL_HA_DT_CMD_WRITE,
} ctl_ha_dt_cmd;

struct ctl_ha_dt_req;

typedef void (*ctl_ha_dt_cb)(struct ctl_ha_dt_req *);

struct ctl_ha_dt_req {
	ctl_ha_dt_cmd	command;
	void		*context;
	ctl_ha_dt_cb	callback;
	ctl_ha_dtid	id;
	int		ret;
	uint32_t	size;
	uint8_t		*local;
	uint8_t		*remote;
};

typedef void (*ctl_evt_handler)(ctl_ha_channel channel, ctl_ha_event event,
				int param);
void ctl_ha_register_evthandler(ctl_ha_channel channel,
				ctl_evt_handler handler);

static inline ctl_ha_status
ctl_ha_msg_create(ctl_ha_channel channel, ctl_evt_handler handler)
{
	return (CTL_HA_STATUS_SUCCESS);
}

/*
 * Receive a message of the specified size.
 */
static inline ctl_ha_status
ctl_ha_msg_recv(ctl_ha_channel channel, void *buffer, unsigned int size,
		int wait)
{
	return (CTL_HA_STATUS_SUCCESS);
}

/*
 * Send a message of the specified size.
 */
static inline ctl_ha_status
ctl_ha_msg_send(ctl_ha_channel channel, void *buffer, unsigned int size,
		int wait)
{
	return (CTL_HA_STATUS_SUCCESS);
}

/*
 * Allocate a data transfer request structure.
 */
static inline struct ctl_ha_dt_req *
ctl_dt_req_alloc(void)
{
	return (NULL);
}

/*
 * Free a data transfer request structure.
 */
static inline void
ctl_dt_req_free(struct ctl_ha_dt_req *req)
{
	return;
}

/*
 * Issue a DMA request for a single buffer.
 */
static inline ctl_ha_status
ctl_dt_single(struct ctl_ha_dt_req *req)
{
	return (CTL_HA_STATUS_WAIT);
}

/*
 * SINGLE:	   One node
 * HA:		   Two nodes (Active/Active implied)
 * SLAVE/MASTER:   The component can set these flags to indicate which side
 *		   is in control.  It has no effect on the HA framework.
 */
typedef enum {
	CTL_HA_STATE_UNKNOWN	= 0x00,
	CTL_HA_STATE_SINGLE	= 0x01,
	CTL_HA_STATE_HA		= 0x02,
	CTL_HA_STATE_MASK	= 0x0F,
	CTL_HA_STATE_SLAVE	= 0x10,
	CTL_HA_STATE_MASTER	= 0x20
} ctl_ha_state;

typedef enum {
	CTL_HA_COMP_STATUS_OK,
	CTL_HA_COMP_STATUS_FAILED,
	CTL_HA_COMP_STATUS_ERROR
} ctl_ha_comp_status;

struct ctl_ha_component;

typedef ctl_ha_comp_status (*ctl_hacmp_init_t)(struct ctl_ha_component *);
typedef ctl_ha_comp_status (*ctl_hacmp_start_t)(struct ctl_ha_component *,
						ctl_ha_state);

struct ctl_ha_component {
	char			*name;
	ctl_ha_state 		state;
	ctl_ha_comp_status	status;
	ctl_hacmp_init_t	init;
	ctl_hacmp_start_t	start;
	ctl_hacmp_init_t	quiesce;
};

#define	CTL_HA_STATE_IS_SINGLE(state)	((state & CTL_HA_STATE_MASK) == \
					  CTL_HA_STATE_SINGLE)
#define	CTL_HA_STATE_IS_HA(state)	((state & CTL_HA_STATE_MASK) == \
					  CTL_HA_STATE_HA)

#endif /* _CTL_HA_H_ */
