//
// Created by tnoulens on 11/6/23.
//

#include "malcolm.h"

static int is_valid_ip(char *ip)
{
	int		dots;
	char	*tmp;
	int		nb_bits;

	tmp = ip;
	dots = 0;
	if (ft_strlen(tmp) > 15)
		return (0);
	if (ip[0] == '.' || ip[ft_strlen(ip) - 1] == '.')
		return (0);
	while (*tmp)
	{
		if (*tmp == '.' && *(tmp + 1) != '.')
			dots++;
		tmp++;
	}
	if (dots != 3)
		return (0);
	tmp = ip;
	nb_bits = 0;
	while (dots-- >= 0)
	{
		nb_bits = ft_atoi(tmp);
		if (nb_bits < 0 || nb_bits > 255)
			return (0);
		while (*tmp && *tmp != '.')
			tmp++;
		tmp++;
	}
	return (1);
}

static int is_valid_mac(char *mac)
{
	int		d_dots;
	char	*tmp;
	int		nb_bits;

	tmp = mac;
	d_dots = 0;
	if (ft_strlen(tmp) > 17)
		return (0);
	if (mac[0] == ':' || mac[ft_strlen(mac) - 1] == ':')
		return (0);
	while (*tmp)
	{
		if (*tmp == ':' && *(tmp + 1) != ':')
			d_dots++;
		tmp++;
	}
	if (d_dots != 5)
		return (0);
	tmp = mac;
	nb_bits = 0;
	while (d_dots-- >= 0)
	{
		nb_bits = check_hex(tmp);
		if (nb_bits < 0x00 || nb_bits > 0xff)
			return (0);
		while (*tmp && *tmp != ':')
			tmp++;
		tmp++;
	}
	return (1);
}

void    init_checks(int argc, char **argv)
{
	// Check if the user is root
	if (getuid() != 0)
	{
		fprintf(stderr, RED"Error: You must be root to run this program\n"RESET);
		exit(EXIT_FAILURE);
	}
    // Check if the user gave the right number of arguments
    if ((argc != 5 && argc != 6) || (argc == 6 && ft_strcmp(argv[5], "--verbose") != 0))
    {
		fprintf(stderr, RED"Usage: %s <source ip> <source MAC addr> <target ip> <target MAC addr>\n"RESET, argv[0]);
		exit(EXIT_FAILURE);
    }
    // Check if the user gave a valid IP addresses and valid MAC addresses
    if (!is_valid_ip(argv[1]))
    {
		fprintf(stderr, RED"Error: Invalid source IP address\n"RESET);
		exit(EXIT_FAILURE);
    }
    if (!is_valid_ip(argv[3]))
    {
		fprintf(stderr, RED"Error: Invalid target IP address\n"RESET);
		exit(EXIT_FAILURE);
    }
	if (!is_valid_mac(argv[2]))
	{
		fprintf(stderr, RED"Error: Invalid source MAC address\n"RESET);
		exit(EXIT_FAILURE);
	}
	if (!is_valid_mac(argv[4]))
	{
		fprintf(stderr, RED"Error: Invalid target MAC address\n"RESET);
		exit(EXIT_FAILURE);
	}
}
