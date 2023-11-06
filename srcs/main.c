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
		fprintf(stderr, YELLOW"\nSIGINT received\n"RESET);
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
	g_status = -1;

	signal_handling();
    init_checks(argc, argv);
    printf("Source UID is: %d\n", getuid());
	printf("Waiting for ARP request...\n\n");
	while (1)
	{
		sleep(1000);
	}
	return (0);
}