/*
 * Copyright (c) 2013-2014 Qlogic Corporation
 * All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File: ql_isr.c
 * Author : David C Somayajulu, Qlogic Corporation, Aliso Viejo, CA 92656.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/sys/dev/qlxgbe/ql_isr.c 251605 2013-06-10 17:12:22Z davidcs $");


#include "ql_os.h"
#include "ql_hw.h"
#include "ql_def.h"
#include "ql_inline.h"
#include "ql_ver.h"
#include "ql_glbl.h"
#include "ql_dbg.h"

static void qla_replenish_normal_rx(qla_host_t *ha, qla_sds_t *sdsp,
		uint32_t r_idx);

static void
qla_rcv_error(qla_host_t *ha)
{
	ha->flags.stop_rcv = 1;
	ha->qla_initiate_recovery = 1;
}


/*
 * Name: qla_rx_intr
 * Function: Handles normal ethernet frames received
 */
static void
qla_rx_intr(qla_host_t *ha, qla_sgl_rcv_t *sgc, uint32_t sds_idx)
{
	qla_rx_buf_t		*rxb;
	struct mbuf		*mp = NULL, *mpf = NULL, *mpl = NULL;
	struct ifnet		*ifp = ha->ifp;
	qla_sds_t		*sdsp;
	struct ether_vlan_header *eh;
	uint32_t		i, rem_len = 0;
	uint32_t		r_idx = 0;
	qla_rx_ring_t		*rx_ring;

	if (ha->hw.num_rds_rings > 1)
		r_idx = sds_idx;
	
	ha->hw.rds[r_idx].count++;

	sdsp = &ha->hw.sds[sds_idx];
	rx_ring = &ha->rx_ring[r_idx];
	
	for (i = 0; i < sgc->num_handles; i++) {
		rxb = &rx_ring->rx_buf[sgc->handle[i] & 0x7FFF];

		QL_ASSERT(ha, (rxb != NULL),
			("%s: [sds_idx]=[%d] rxb != NULL\n", __func__,\
			sds_idx));

		if ((rxb == NULL) || QL_ERR_INJECT(ha, INJCT_RX_RXB_INVAL)) {
			/* log the error */
			device_printf(ha->pci_dev,
				"%s invalid rxb[%d, %d, 0x%04x]\n",
				__func__, sds_idx, i, sgc->handle[i]);
			qla_rcv_error(ha);
			return;
		}

		mp = rxb->m_head;
		if (i == 0) 
			mpf = mp;

		QL_ASSERT(ha, (mp != NULL),
			("%s: [sds_idx]=[%d] mp != NULL\n", __func__,\
			sds_idx));

		bus_dmamap_sync(ha->rx_tag, rxb->map, BUS_DMASYNC_POSTREAD);

		rxb->m_head = NULL;
		rxb->next = sdsp->rxb_free;
		sdsp->rxb_free = rxb;
		sdsp->rx_free++;
	
		if ((mp == NULL) || QL_ERR_INJECT(ha, INJCT_RX_MP_NULL)) {
			/* log the error */
			device_printf(ha->pci_dev,
				"%s mp  == NULL [%d, %d, 0x%04x]\n",
				__func__, sds_idx, i, sgc->handle[i]);
			qla_rcv_error(ha);
			return;
		}

		if (i == 0) {
			mpl = mpf = mp;
			mp->m_flags |= M_PKTHDR;
			mp->m_pkthdr.len = sgc->pkt_length;
			mp->m_pkthdr.rcvif = ifp;
			rem_len = mp->m_pkthdr.len;
		} else {
			mp->m_flags &= ~M_PKTHDR;
			mpl->m_next = mp;
			mpl = mp;
			rem_len = rem_len - mp->m_len;
		}
	}

	mpl->m_len = rem_len;

	eh = mtod(mpf, struct ether_vlan_header *);

	if (eh->evl_encap_proto == htons(ETHERTYPE_VLAN)) {
		uint32_t *data = (uint32_t *)eh;

		mpf->m_pkthdr.ether_vtag = ntohs(eh->evl_tag);
		mpf->m_flags |= M_VLANTAG;

		*(data + 3) = *(data + 2);
		*(data + 2) = *(data + 1);
		*(data + 1) = *data;

		m_adj(mpf, ETHER_VLAN_ENCAP_LEN);
	}

	if (sgc->chksum_status == Q8_STAT_DESC_STATUS_CHKSUM_OK) {
		mpf->m_pkthdr.csum_flags = CSUM_IP_CHECKED | CSUM_IP_VALID |
			CSUM_DATA_VALID | CSUM_PSEUDO_HDR;
		mpf->m_pkthdr.csum_data = 0xFFFF;
	} else {
		mpf->m_pkthdr.csum_flags = 0;
	}

	ifp->if_ipackets++;

	mpf->m_pkthdr.flowid = sgc->rss_hash;
	mpf->m_flags |= M_FLOWID;

	(*ifp->if_input)(ifp, mpf);

	if (sdsp->rx_free > ha->std_replenish)
		qla_replenish_normal_rx(ha, sdsp, r_idx);

	return;
}

#define QLA_TCP_HDR_SIZE        20
#define QLA_TCP_TS_OPTION_SIZE  12

/*
 * Name: qla_lro_intr
 * Function: Handles normal ethernet frames received
 */
static int
qla_lro_intr(qla_host_t *ha, qla_sgl_lro_t *sgc, uint32_t sds_idx)
{
	qla_rx_buf_t *rxb;
	struct mbuf *mp = NULL, *mpf = NULL, *mpl = NULL;
	struct ifnet *ifp = ha->ifp;
	qla_sds_t *sdsp;
	struct ether_vlan_header *eh;
	uint32_t i, rem_len = 0, pkt_length, iplen;
	struct tcphdr *th;
	struct ip *ip = NULL;
	struct ip6_hdr *ip6 = NULL;
	uint16_t etype;
	uint32_t r_idx = 0;
	qla_rx_ring_t *rx_ring;

	if (ha->hw.num_rds_rings > 1)
		r_idx = sds_idx;

	ha->hw.rds[r_idx].count++;

	rx_ring = &ha->rx_ring[r_idx];
	
	ha->lro_pkt_count++;

	sdsp = &ha->hw.sds[sds_idx];
	
	pkt_length = sgc->payload_length + sgc->l4_offset;

	if (sgc->flags & Q8_LRO_COMP_TS) {
		pkt_length += QLA_TCP_HDR_SIZE + QLA_TCP_TS_OPTION_SIZE;
	} else {
		pkt_length += QLA_TCP_HDR_SIZE;
	}
	ha->lro_bytes += pkt_length;

	for (i = 0; i < sgc->num_handles; i++) {
		rxb = &rx_ring->rx_buf[sgc->handle[i] & 0x7FFF];

		QL_ASSERT(ha, (rxb != NULL),
			("%s: [sds_idx]=[%d] rxb != NULL\n", __func__,\
			sds_idx));

		if ((rxb == NULL) || QL_ERR_INJECT(ha, INJCT_LRO_RXB_INVAL)) {
			/* log the error */
			device_printf(ha->pci_dev,
				"%s invalid rxb[%d, %d, 0x%04x]\n",
				__func__, sds_idx, i, sgc->handle[i]);
			qla_rcv_error(ha);
			return (0);
		}

		mp = rxb->m_head;
		if (i == 0) 
			mpf = mp;

		QL_ASSERT(ha, (mp != NULL),
			("%s: [sds_idx]=[%d] mp != NULL\n", __func__,\
			sds_idx));

		bus_dmamap_sync(ha->rx_tag, rxb->map, BUS_DMASYNC_POSTREAD);

		rxb->m_head = NULL;
		rxb->next = sdsp->rxb_free;
		sdsp->rxb_free = rxb;
		sdsp->rx_free++;
	
		if ((mp == NULL) || QL_ERR_INJECT(ha, INJCT_LRO_MP_NULL)) {
			/* log the error */
			device_printf(ha->pci_dev,
				"%s mp  == NULL [%d, %d, 0x%04x]\n",
				__func__, sds_idx, i, sgc->handle[i]);
			qla_rcv_error(ha);
			return (0);
		}

		if (i == 0) {
			mpl = mpf = mp;
			mp->m_flags |= M_PKTHDR;
			mp->m_pkthdr.len = pkt_length;
			mp->m_pkthdr.rcvif = ifp;
			rem_len = mp->m_pkthdr.len;
		} else {
			mp->m_flags &= ~M_PKTHDR;
			mpl->m_next = mp;
			mpl = mp;
			rem_len = rem_len - mp->m_len;
		}
	}

	mpl->m_len = rem_len;

	th = (struct tcphdr *)(mpf->m_data + sgc->l4_offset);

	if (sgc->flags & Q8_LRO_COMP_PUSH_BIT)
		th->th_flags |= TH_PUSH;

	m_adj(mpf, sgc->l2_offset);

	eh = mtod(mpf, struct ether_vlan_header *);

	if (eh->evl_encap_proto == htons(ETHERTYPE_VLAN)) {
		uint32_t *data = (uint32_t *)eh;

		mpf->m_pkthdr.ether_vtag = ntohs(eh->evl_tag);
		mpf->m_flags |= M_VLANTAG;

		*(data + 3) = *(data + 2);
		*(data + 2) = *(data + 1);
		*(data + 1) = *data;

		m_adj(mpf, ETHER_VLAN_ENCAP_LEN);

		etype = ntohs(eh->evl_proto);
	} else {
		etype = ntohs(eh->evl_encap_proto);
	}

	if (etype == ETHERTYPE_IP) {
		ip = (struct ip *)(mpf->m_data + ETHER_HDR_LEN);
	
		iplen = (ip->ip_hl << 2) + (th->th_off << 2) +
				sgc->payload_length;

                ip->ip_len = htons(iplen);

		ha->ipv4_lro++;
	} else if (etype == ETHERTYPE_IPV6) {
		ip6 = (struct ip6_hdr *)(mpf->m_data + ETHER_HDR_LEN);

		iplen = (th->th_off << 2) + sgc->payload_length;

		ip6->ip6_plen = htons(iplen);

		ha->ipv6_lro++;
	} else {
		m_freem(mpf);

		if (sdsp->rx_free > ha->std_replenish)
			qla_replenish_normal_rx(ha, sdsp, r_idx);
		return 0;
	}

	mpf->m_pkthdr.csum_flags = CSUM_IP_CHECKED | CSUM_IP_VALID |
					CSUM_DATA_VALID | CSUM_PSEUDO_HDR;
	mpf->m_pkthdr.csum_data = 0xFFFF;

	mpf->m_pkthdr.flowid = sgc->rss_hash;
	mpf->m_flags |= M_FLOWID;

	ifp->if_ipackets++;

	(*ifp->if_input)(ifp, mpf);

	if (sdsp->rx_free > ha->std_replenish)
		qla_replenish_normal_rx(ha, sdsp, r_idx);

	return (0);
}

static int
qla_rcv_cont_sds(qla_host_t *ha, uint32_t sds_idx, uint32_t comp_idx,
	uint32_t dcount, uint16_t *handle, uint16_t *nhandles)
{
	uint32_t i;
	uint16_t num_handles;
	q80_stat_desc_t *sdesc;
	uint32_t opcode;

	*nhandles = 0;
	dcount--;

	for (i = 0; i < dcount; i++) {
		comp_idx = (comp_idx + 1) & (NUM_STATUS_DESCRIPTORS-1);
		sdesc = (q80_stat_desc_t *)
				&ha->hw.sds[sds_idx].sds_ring_base[comp_idx];

		opcode = Q8_STAT_DESC_OPCODE((sdesc->data[1]));

		if (!opcode) {
			device_printf(ha->pci_dev, "%s: opcode=0 %p %p\n",
				__func__, (void *)sdesc->data[0],
				(void *)sdesc->data[1]);
			return -1;
		}

		num_handles = Q8_SGL_STAT_DESC_NUM_HANDLES((sdesc->data[1]));
		if (!num_handles) {
			device_printf(ha->pci_dev, "%s: opcode=0 %p %p\n",
				__func__, (void *)sdesc->data[0],
				(void *)sdesc->data[1]);
			return -1;
		}

		if (QL_ERR_INJECT(ha, INJCT_NUM_HNDLE_INVALID))
			num_handles = -1;

		switch (num_handles) {

		case 1:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			break;

		case 2:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			break;

		case 3:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE3((sdesc->data[0]));
			break;

		case 4:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE3((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE4((sdesc->data[0]));
			break;

		case 5:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE3((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE4((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE5((sdesc->data[1]));
			break;

		case 6:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE3((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE4((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE5((sdesc->data[1]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE6((sdesc->data[1]));
			break;

		case 7:
			*handle++ = Q8_SGL_STAT_DESC_HANDLE1((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE2((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE3((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE4((sdesc->data[0]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE5((sdesc->data[1]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE6((sdesc->data[1]));
			*handle++ = Q8_SGL_STAT_DESC_HANDLE7((sdesc->data[1]));
			break;

		default:
			device_printf(ha->pci_dev,
				"%s: invalid num handles %p %p\n",
				__func__, (void *)sdesc->data[0],
				(void *)sdesc->data[1]);

			QL_ASSERT(ha, (0),\
			("%s: %s [nh, sds, d0, d1]=[%d, %d, %p, %p]\n",
			__func__, "invalid num handles", sds_idx, num_handles,
			(void *)sdesc->data[0],(void *)sdesc->data[1]));

			qla_rcv_error(ha);
			return 0;
		}
		*nhandles = *nhandles + num_handles;
	}
	return 0;
}

/*
 * Name: qla_rcv_isr
 * Function: Main Interrupt Service Routine
 */
static uint32_t
qla_rcv_isr(qla_host_t *ha, uint32_t sds_idx, uint32_t count)
{
	device_t dev;
	qla_hw_t *hw;
	uint32_t comp_idx, c_idx = 0, desc_count = 0, opcode;
	volatile q80_stat_desc_t *sdesc, *sdesc0 = NULL;
	uint32_t ret = 0;
	qla_sgl_comp_t sgc;
	uint16_t nhandles;
	uint32_t sds_replenish_threshold = 0;

	dev = ha->pci_dev;
	hw = &ha->hw;

	hw->sds[sds_idx].rcv_active = 1;
	if (ha->flags.stop_rcv) {
		hw->sds[sds_idx].rcv_active = 0;
		return 0;
	}

	QL_DPRINT2(ha, (dev, "%s: [%d]enter\n", __func__, sds_idx));

	/*
	 * receive interrupts
	 */
	comp_idx = hw->sds[sds_idx].sdsr_next;

	while (count-- && !ha->flags.stop_rcv) {

		sdesc = (q80_stat_desc_t *)
				&hw->sds[sds_idx].sds_ring_base[comp_idx];

		opcode = Q8_STAT_DESC_OPCODE((sdesc->data[1]));

		if (!opcode)
			break;

		hw->sds[sds_idx].intr_count++;
		switch (opcode) {

		case Q8_STAT_DESC_OPCODE_RCV_PKT:

			desc_count = 1;

			bzero(&sgc, sizeof(qla_sgl_comp_t));

			sgc.rcv.pkt_length =
				Q8_STAT_DESC_TOTAL_LENGTH((sdesc->data[0]));
			sgc.rcv.num_handles = 1;
			sgc.rcv.handle[0] =
				Q8_STAT_DESC_HANDLE((sdesc->data[0]));
			sgc.rcv.chksum_status =
				Q8_STAT_DESC_STATUS((sdesc->data[1]));

			sgc.rcv.rss_hash =
				Q8_STAT_DESC_RSS_HASH((sdesc->data[0]));

			if (Q8_STAT_DESC_VLAN((sdesc->data[1]))) {
				sgc.rcv.vlan_tag =
					Q8_STAT_DESC_VLAN_ID((sdesc->data[1]));
			}
			qla_rx_intr(ha, &sgc.rcv, sds_idx);
			break;

		case Q8_STAT_DESC_OPCODE_SGL_RCV:

			desc_count =
				Q8_STAT_DESC_COUNT_SGL_RCV((sdesc->data[1]));

			if (desc_count > 1) {
				c_idx = (comp_idx + desc_count -1) &
						(NUM_STATUS_DESCRIPTORS-1);
				sdesc0 = (q80_stat_desc_t *)
					&hw->sds[sds_idx].sds_ring_base[c_idx];

				if (Q8_STAT_DESC_OPCODE((sdesc0->data[1])) !=
						Q8_STAT_DESC_OPCODE_CONT) {
					desc_count = 0;
					break;
				}
			}

			bzero(&sgc, sizeof(qla_sgl_comp_t));

			sgc.rcv.pkt_length =
				Q8_STAT_DESC_TOTAL_LENGTH_SGL_RCV(\
					(sdesc->data[0]));
			sgc.rcv.chksum_status =
				Q8_STAT_DESC_STATUS((sdesc->data[1]));

			sgc.rcv.rss_hash =
				Q8_STAT_DESC_RSS_HASH((sdesc->data[0]));

			if (Q8_STAT_DESC_VLAN((sdesc->data[1]))) {
				sgc.rcv.vlan_tag =
					Q8_STAT_DESC_VLAN_ID((sdesc->data[1]));
			}

			QL_ASSERT(ha, (desc_count <= 2) ,\
				("%s: [sds_idx, data0, data1]="\
				"%d, %p, %p]\n", __func__, sds_idx,\
				(void *)sdesc->data[0],\
				(void *)sdesc->data[1]));

			sgc.rcv.num_handles = 1;
			sgc.rcv.handle[0] = 
				Q8_STAT_DESC_HANDLE((sdesc->data[0]));
			
			if (qla_rcv_cont_sds(ha, sds_idx, comp_idx, desc_count,
				&sgc.rcv.handle[1], &nhandles)) {
				device_printf(dev,
					"%s: [sds_idx, dcount, data0, data1]="
					 "[%d, %d, 0x%llx, 0x%llx]\n",
					__func__, sds_idx, desc_count,
					(long long unsigned int)sdesc->data[0],
					(long long unsigned int)sdesc->data[1]);
				desc_count = 0;
				break;	
			}

			sgc.rcv.num_handles += nhandles;

			qla_rx_intr(ha, &sgc.rcv, sds_idx);
			
			break;

		case Q8_STAT_DESC_OPCODE_SGL_LRO:

			desc_count =
				Q8_STAT_DESC_COUNT_SGL_LRO((sdesc->data[1]));

			if (desc_count > 1) {
				c_idx = (comp_idx + desc_count -1) &
						(NUM_STATUS_DESCRIPTORS-1);
				sdesc0 = (q80_stat_desc_t *)
					&hw->sds[sds_idx].sds_ring_base[c_idx];

				if (Q8_STAT_DESC_OPCODE((sdesc0->data[1])) !=
						Q8_STAT_DESC_OPCODE_CONT) {
					desc_count = 0;
					break;
				}
			}
			bzero(&sgc, sizeof(qla_sgl_comp_t));

			sgc.lro.payload_length =
			Q8_STAT_DESC_TOTAL_LENGTH_SGL_RCV((sdesc->data[0]));
				
			sgc.lro.rss_hash =
				Q8_STAT_DESC_RSS_HASH((sdesc->data[0]));
			
			sgc.lro.num_handles = 1;
			sgc.lro.handle[0] =
				Q8_STAT_DESC_HANDLE((sdesc->data[0]));

			if (Q8_SGL_LRO_STAT_TS((sdesc->data[1])))
				sgc.lro.flags |= Q8_LRO_COMP_TS;

			if (Q8_SGL_LRO_STAT_PUSH_BIT((sdesc->data[1])))
				sgc.lro.flags |= Q8_LRO_COMP_PUSH_BIT;

			sgc.lro.l2_offset =
				Q8_SGL_LRO_STAT_L2_OFFSET((sdesc->data[1]));
			sgc.lro.l4_offset =
				Q8_SGL_LRO_STAT_L4_OFFSET((sdesc->data[1]));

			if (Q8_STAT_DESC_VLAN((sdesc->data[1]))) {
				sgc.lro.vlan_tag =
					Q8_STAT_DESC_VLAN_ID((sdesc->data[1]));
			}

			QL_ASSERT(ha, (desc_count <= 7) ,\
				("%s: [sds_idx, data0, data1]="\
				 "[%d, 0x%llx, 0x%llx]\n",\
				__func__, sds_idx,\
				(long long unsigned int)sdesc->data[0],\
				(long long unsigned int)sdesc->data[1]));
				
			if (qla_rcv_cont_sds(ha, sds_idx, comp_idx, 
				desc_count, &sgc.lro.handle[1], &nhandles)) {
				device_printf(dev,
				"%s: [sds_idx, data0, data1]="\
				 "[%d, 0x%llx, 0x%llx]\n",\
				__func__, sds_idx,\
				(long long unsigned int)sdesc->data[0],\
				(long long unsigned int)sdesc->data[1]);

				desc_count = 0;
				break;	
			}

			sgc.lro.num_handles += nhandles;

			if (qla_lro_intr(ha, &sgc.lro, sds_idx)) {
				device_printf(dev,
				"%s: [sds_idx, data0, data1]="\
				 "[%d, 0x%llx, 0x%llx]\n",\
				__func__, sds_idx,\
				(long long unsigned int)sdesc->data[0],\
				(long long unsigned int)sdesc->data[1]);
				device_printf(dev,
				"%s: [comp_idx, c_idx, dcount, nhndls]="\
				 "[%d, %d, %d, %d]\n",\
				__func__, comp_idx, c_idx, desc_count,
				sgc.lro.num_handles);
				if (desc_count > 1) {
				device_printf(dev,
				"%s: [sds_idx, data0, data1]="\
				 "[%d, 0x%llx, 0x%llx]\n",\
				__func__, sds_idx,\
				(long long unsigned int)sdesc0->data[0],\
				(long long unsigned int)sdesc0->data[1]);
				}
			}
			
			break;

		default:
			device_printf(dev, "%s: default 0x%llx!\n", __func__,
					(long long unsigned int)sdesc->data[0]);
			break;
		}

		if (desc_count == 0)
			break;

		sds_replenish_threshold += desc_count;


		while (desc_count--) {
			sdesc->data[0] = 0ULL;
			sdesc->data[1] = 0ULL;
			comp_idx = (comp_idx + 1) & (NUM_STATUS_DESCRIPTORS-1);
			sdesc = (q80_stat_desc_t *)
				&hw->sds[sds_idx].sds_ring_base[comp_idx];
		}

		if (sds_replenish_threshold > ha->hw.sds_cidx_thres) {
			sds_replenish_threshold = 0;
			if (hw->sds[sds_idx].sdsr_next != comp_idx) {
				QL_UPDATE_SDS_CONSUMER_INDEX(ha, sds_idx,\
					comp_idx);
			}
			hw->sds[sds_idx].sdsr_next = comp_idx;
		}
	}

	if (ha->flags.stop_rcv)
		goto qla_rcv_isr_exit;

	if (hw->sds[sds_idx].sdsr_next != comp_idx) {
		QL_UPDATE_SDS_CONSUMER_INDEX(ha, sds_idx, comp_idx);
	}
	hw->sds[sds_idx].sdsr_next = comp_idx;

	sdesc = (q80_stat_desc_t *)&hw->sds[sds_idx].sds_ring_base[comp_idx];
	opcode = Q8_STAT_DESC_OPCODE((sdesc->data[1]));

	if (opcode)
		ret = -1;

qla_rcv_isr_exit:
	hw->sds[sds_idx].rcv_active = 0;

	return (ret);
}

void
ql_mbx_isr(void *arg)
{
	qla_host_t *ha;
	uint32_t data;
	uint32_t prev_link_state;

	ha = arg;

	if (ha == NULL) {
		device_printf(ha->pci_dev, "%s: arg == NULL\n", __func__);
		return;
	}

	data = READ_REG32(ha, Q8_FW_MBOX_CNTRL);
	if ((data & 0x3) != 0x1) {
		WRITE_REG32(ha, ha->hw.mbx_intr_mask_offset, 0);
		return;
	}

	data = READ_REG32(ha, Q8_FW_MBOX0);

	if ((data & 0xF000) != 0x8000)
		return;

	data = data & 0xFFFF;

	switch (data) {

	case 0x8001:  /* It's an AEN */
		
		ha->hw.cable_oui = READ_REG32(ha, (Q8_FW_MBOX0 + 4));

		data = READ_REG32(ha, (Q8_FW_MBOX0 + 8));
		ha->hw.cable_length = data & 0xFFFF;

		data = data >> 16;
		ha->hw.link_speed = data & 0xFFF;

		data = READ_REG32(ha, (Q8_FW_MBOX0 + 12));

		prev_link_state =  ha->hw.link_up;
		ha->hw.link_up = (((data & 0xFF) == 0) ? 0 : 1);

		if (prev_link_state !=  ha->hw.link_up) {
			if (ha->hw.link_up)
				if_link_state_change(ha->ifp, LINK_STATE_UP);
			else
				if_link_state_change(ha->ifp, LINK_STATE_DOWN);
		}


		ha->hw.module_type = ((data >> 8) & 0xFF);
		ha->hw.flags.fduplex = (((data & 0xFF0000) == 0) ? 0 : 1);
		ha->hw.flags.autoneg = (((data & 0xFF000000) == 0) ? 0 : 1);
		
		data = READ_REG32(ha, (Q8_FW_MBOX0 + 16));
		ha->hw.flags.loopback_mode = data & 0x03;

		ha->hw.link_faults = (data >> 3) & 0xFF;

		WRITE_REG32(ha, Q8_FW_MBOX_CNTRL, 0x0);
		WRITE_REG32(ha, ha->hw.mbx_intr_mask_offset, 0x0);
		break;

	default:
		device_printf(ha->pci_dev, "%s: AEN[0x%08x]\n", __func__, data);
		WRITE_REG32(ha, Q8_FW_MBOX_CNTRL, 0x0);
		WRITE_REG32(ha, ha->hw.mbx_intr_mask_offset, 0x0);
		break;
	}
	return;
}


static void
qla_replenish_normal_rx(qla_host_t *ha, qla_sds_t *sdsp, uint32_t r_idx)
{
	qla_rx_buf_t *rxb;
	int count = sdsp->rx_free;
	uint32_t rx_next;
	qla_rdesc_t *rdesc;

	/* we can play with this value via a sysctl */
	uint32_t replenish_thresh = ha->hw.rds_pidx_thres;

	rdesc = &ha->hw.rds[r_idx];

	rx_next = rdesc->rx_next;

	while (count--) {
		rxb = sdsp->rxb_free;

		if (rxb == NULL)
			break;

		sdsp->rxb_free = rxb->next;
		sdsp->rx_free--;

		if (ql_get_mbuf(ha, rxb, NULL) == 0) {
			qla_set_hw_rcv_desc(ha, r_idx, rdesc->rx_in,
				rxb->handle,
				rxb->paddr, (rxb->m_head)->m_pkthdr.len);
			rdesc->rx_in++;
			if (rdesc->rx_in == NUM_RX_DESCRIPTORS)
				rdesc->rx_in = 0;
			rdesc->rx_next++;
			if (rdesc->rx_next == NUM_RX_DESCRIPTORS)
				rdesc->rx_next = 0;
		} else {
			device_printf(ha->pci_dev,
				"%s: ql_get_mbuf [0,(%d),(%d)] failed\n",
				__func__, rdesc->rx_in, rxb->handle);

			rxb->m_head = NULL;
			rxb->next = sdsp->rxb_free;
			sdsp->rxb_free = rxb;
			sdsp->rx_free++;

			break;
		}
		if (replenish_thresh-- == 0) {
			QL_UPDATE_RDS_PRODUCER_INDEX(ha, rdesc->prod_std,
				rdesc->rx_next);
			rx_next = rdesc->rx_next;
			replenish_thresh = ha->hw.rds_pidx_thres;
		}
	}

	if (rx_next != rdesc->rx_next) {
		QL_UPDATE_RDS_PRODUCER_INDEX(ha, rdesc->prod_std,
			rdesc->rx_next);
	}
}

void
ql_isr(void *arg)
{
	qla_ivec_t *ivec = arg;
	qla_host_t *ha ;
	int idx;
	qla_hw_t *hw;
	struct ifnet *ifp;
	uint32_t ret = 0;

	ha = ivec->ha;
	hw = &ha->hw;
	ifp = ha->ifp;

	if ((idx = ivec->sds_idx) >= ha->hw.num_sds_rings)
		return;

	if (idx == 0)
		taskqueue_enqueue(ha->tx_tq, &ha->tx_task);
	
	ret = qla_rcv_isr(ha, idx, -1);

	if (idx == 0)
		taskqueue_enqueue(ha->tx_tq, &ha->tx_task);

	if (!ha->flags.stop_rcv) {
		QL_ENABLE_INTERRUPTS(ha, idx);
	}
	return;
}

