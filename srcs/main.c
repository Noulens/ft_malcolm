//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int g_status;

void	tmp_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		if (g_status != -1 && close(g_status) == -1)
			error("Error: close() failed: ", errno);
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
	void    *data;
	int     sd_socket;
	char *test;

	welcome();
	signal_handling();
	init_checks(argc, argv);
	sd_socket = socket(AF_PACKET, SOCK_RAW, ETH_P_ARP);
	if (g_status = sd_socket, sd_socket == -1)
		error("socket() failed", errno);
/*	getMacAddress("enp0s3", sd_socket);*/
	printf("Waiting for ARP request...\n\n");
	while (1)
	{
		data = parse(argv);
		poison(data);
		sleep(1);
	}
	return (0);
}