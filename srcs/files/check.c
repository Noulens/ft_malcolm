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
		error("You must be root to run this program", -1, TRUE);
    // Check if the user gave the right number of arguments
	if (BONUS == TRUE)
	{
		if ((argc != 5 && argc != 6) || (argc == 6 && ft_strcmp(argv[5], "--verbose") != 0))
			error("Usage: ./ft_malcolm <source ip> <source MAC addr> <target ip> <target MAC addr> [--verbose]", -1, TRUE);
	}
	else if (argc != 5)
	    error("Usage: ./ft_malcolm <source ip> <source MAC addr> <target ip> <target MAC addr>", -1, TRUE);
    // Check if the user gave valid IP addresses
    if (!is_valid_ip(argv[1]))
		error("Invalid source IP address", -1, TRUE);
    if (!is_valid_ip(argv[3]))
	    error("Invalid target IP address", -1, TRUE);
	// Check if the user gave valid MAC addresses
	if (!is_valid_mac(argv[2]))
		error("Invalid source MAC address", -1, TRUE);
	if (!is_valid_mac(argv[4]))
		error("Invalid target MAC address", -1, TRUE);
}
