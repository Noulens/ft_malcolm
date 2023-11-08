//
// Created by tnoulens on 11/7/23.
//

#include "malcolm.h"

void    error(const char *msg, int error_code, int must_exit)
{
	if (error_code != -1)
	{
		fprintf(stderr, RED"Error: %s: %s\n"RESET, msg, strerror(error_code));
	}
	else
		fprintf(stderr, RED"Error: %s\n"RESET, msg);
	if (g_packet_socket > -1 && close(g_packet_socket) <= -1)
		fprintf(stderr, "Error: close() failed: %s\n", strerror(errno));
	if (must_exit == TRUE)
		exit(EXIT_FAILURE);
}
