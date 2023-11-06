//
// Created by tnoulens on 11/6/23.
//

#include "malcolm.h"

void    init_checks(int argc, char **argv)
{
	// Check if the user is root
	if (getuid() != 0)
	{
		fprintf(stderr, RED"Error: You must be root to run this program\n"RESET);
		exit(EXIT_FAILURE);
	}
    (void)argv;
    if (argc != 5)
    {
        fprintf(stderr, RED"Usage: %s <source ip> <source MAC addr> <target ip> <target MAC addr>\n"RESET, argv[0]);
        exit(EXIT_FAILURE);
    }
}
