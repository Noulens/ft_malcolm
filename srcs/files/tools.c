//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int check_hex(char *tmp)
{
	int     hex_value = -1;
	int     i = 0;
	char    *chunk;

	while (tmp[i] != ':' && tmp[i] != '\0')
	{
		if (!(tmp[i] >= '0' && tmp[i] <= '9')
			&& !(tmp[i] >= 'a' && tmp[i] <= 'f')
			&& !(tmp[i] >= 'A' && tmp[i] <= 'F'))
			return (-1);
		i++;
	}
	if (i > 2)
		return (-1);
	chunk = ft_substr(tmp, 0, i);
	if (!chunk)
		error("enomem", errno, TRUE);
	hex_value = ft_atoi_base(chunk, 16);
	free(chunk);
	return (hex_value);
}

void    welcome()
{
	printf(CYAN"  __  _                       _           _\n");
	printf(" / _|| |_        _ __   __ _ | | __  ___ | | _ __\n");
	printf("|  _||  _|      | '  \\ / _` || |/ _|/ _ \\| || '  \\\n");
	printf("|_|   \\__|______|_|_|_|\\__/_||_|\\__|\\___/|_||_|_|_|\n");
	printf(GREEN"            A basic ARP poisoning program\n\n"RESET);
}

int    print_data(t_arp_packet *ah, struct ethhdr *eh)
{
	struct sockaddr_in addrSender = {0};
	struct sockaddr_in addrTarget = {0};

	addrSender.sin_addr.s_addr = ah->arp_spa;
	addrSender.sin_family = AF_INET;
	addrTarget.sin_addr.s_addr = ah->arp_dpa;
	addrTarget.sin_family = AF_INET;
	printf("ARP packet content:\n");
	printf("H/D TYPE : %x PROTO TYPE : %x\n", ntohs(ah->arp_hd), ntohs(ah->arp_pr));
	printf("H/D leng : %x PROTO leng : %x\n", ah->arp_hdl, ah->arp_prl);
	printf("OPERATION : %d\n", ntohs(ah->arp_opcode));
	printf("SENDER MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       ah->arp_sha[0],
	       ah->arp_sha[1],
	       ah->arp_sha[2],
	       ah->arp_sha[3],
	       ah->arp_sha[4],
	       ah->arp_sha[5]);
	printf("SENDER IP address: %s\n", inet_ntoa(addrSender.sin_addr));
	printf("TARGET MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       ah->arp_dha[0],
	       ah->arp_dha[1],
	       ah->arp_dha[2],
	       ah->arp_dha[3],
	       ah->arp_dha[4],
	       ah->arp_dha[5]);
	printf("TARGET IP address: %s\n", inet_ntoa(addrTarget.sin_addr));
	if (eh)
	{
		printf("---ETHER HEADER INFORMATION:---\n");
		printf("ETHER DST MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
		       eh->h_dest[0],
		       eh->h_dest[1],
		       eh->h_dest[2],
		       eh->h_dest[3],
		       eh->h_dest[4],
		       eh->h_dest[5]);
		printf("ETHER SRC MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
		       eh->h_source[0],
		       eh->h_source[1],
		       eh->h_source[2],
		       eh->h_source[3],
		       eh->h_source[4],
		       eh->h_source[5]);
	}
	return (1);
}
