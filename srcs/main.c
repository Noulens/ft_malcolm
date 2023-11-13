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
	t_data  data = {0};
    struct ether_arp arp = {0};
  //  const unsigned char ether_broadcast_addr[]= {0xff,0xff,0xff,0xff, 0xff,0xff};
    struct sockaddr_ll addr = {0};

	welcome();
	signal_handling();
	init_checks(argc, argv, &data);

	g_packet_socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
	if (g_packet_socket <= -1)
		error("socket() failed", errno, TRUE);
	if (BONUS == TRUE && argc == 6 && !ft_strcmp(argv[5], "--verbose"))
		getHost(&data);
	else
		interface(&data);

    addr.sll_family = AF_PACKET;
//    addr.sll_halen = ETH_ALEN;
    addr.sll_protocol = htons(ETH_P_ARP);
//    ft_memcpy(addr.sll_addr,ether_broadcast_addr,ETH_ALEN);
    addr.sll_ifindex = data.interface_index;

    printf("Using interface: %s of index: %d\n", data.interface, addr.sll_ifindex);
	printf("\nWaiting for ARP request...\n\n");

    // make ARP header
    arp.arp_hrd = htons(ARPHRD_ETHER);
    arp.arp_pro = htons(ETH_P_IP);
    arp.arp_hln = ETH_ALEN;
    arp.arp_pln = sizeof(in_addr_t);
    arp.arp_op = htons(ARPOP_REQUEST);
    // make ARP data
    ft_memcpy(&arp.arp_tpa,&data.target.sin_addr,sizeof(arp.arp_tpa));
    ft_memcpy(&arp.arp_tha, &data.target_mac, sizeof(arp.arp_tha));
    ft_memcpy(&arp.arp_spa, &data.source.sin_addr, sizeof(arp.arp_spa));
    ft_memcpy(&arp.arp_sha, &data.source_mac, sizeof(arp.arp_sha));

    if (sendto(g_packet_socket, &arp, sizeof(arp), 0, (struct sockaddr *)&addr, sizeof(addr))==-1)
    {
        error("sendto() failed", errno, TRUE);
    }

/*    while (1)
	{
		poison(&data);
		sleep(1);
	}*/
	return (0);
}