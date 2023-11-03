//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

void	tmp_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		printf(YELLOW"SIGINT received\n"RESET);
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
	signal_handling();
	if (argc != 5)
	{
		fprintf(stderr, RED"Usage: %s <source ip> <source MAC addr> <target ip> <target MAC addr>\n"RESET, argv[0]);
		exit(EXIT_FAILURE);
	}
	printf("Waiting for ARP request...\n\n");
	while (1)
	{
		;
	}
	return (0);
}