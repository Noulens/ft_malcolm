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
			fprintf(stderr, RED"\nError: close() failed: %s\n"RESET, strerror(errno));
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
	void    *data;

	welcome();
	g_status = -1;
	signal_handling();
    init_checks(argc, argv);
	printf("Waiting for ARP request...\n\n");
	while (1)
	{
		data = parse(argv);
		poison(data);
		sleep(1000);
	}
	return (0);
}