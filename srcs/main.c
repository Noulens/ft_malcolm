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

	welcome();
	signal_handling();
	init_checks(argc, argv, &data);
	g_packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (g_packet_socket <= -1)
		error("socket() failed", errno, TRUE);
	if (BONUS == TRUE)
	{
		char    mac[18];
		char    *tmp = getMacAddress("enp0s3", g_packet_socket);
		ft_memcpy(mac, tmp, 18);
		free(tmp);
	}
    parse(argv, &data);
	printf("Waiting for ARP request...\n\n");
	while (1)
	{
		poison(&data);
		sleep(1);
	}
	return (0);
}