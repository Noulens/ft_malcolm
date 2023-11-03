//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 5)
	{
		fprintf(stderr, RED"Usage: %s <source ip> <source MAC addr> <target ip> <target MAC addr>\n"RESET, argv[0]);
		exit(EXIT_FAILURE);
	}
	return (0);

}