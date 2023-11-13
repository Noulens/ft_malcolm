//
// Created by tnoulens on 11/6/23.
//

#include "malcolm.h"

static int is_valid_ip(char **ip, t_data *data)
{
	char    *source = ip[1];
	char    *target = ip[3];
	if (BONUS == TRUE)
	{
		if (ft_strnstr(source, ".", ft_strlen(source)) == NULL)
		{
			char    *test = ft_itoa(ft_atoi(source));
			if (!ft_strcmp(test, source))
			{
				data->source.sin_addr.s_addr = htonl((uint32_t)ft_atoi(source));
				free(test);
			}
			else
				return (free(test), 0);
		}
		else
		{
			if (inet_pton(AF_INET, source, &data->source.sin_addr) != 1)
				return (0);
		}
		if (ft_strnstr(target, ".", ft_strlen(target)) == NULL)
		{
			char    *test = ft_itoa(ft_atoi(target));
			if (!ft_strcmp(test, target))
			{
				data->target.sin_addr.s_addr = htonl((uint32_t)ft_atoi(target));
				free(test);
			}
			else
				return (free(test), 0);
		}
		else
		{
			if (inet_pton(AF_INET, target, &data->target.sin_addr) != 1)
				return (0);
		}
	}
	else
	{
		if (inet_pton(AF_INET, source, &(data->source.sin_addr)) != 1
			|| inet_pton(AF_INET, target, &(data->target.sin_addr)) != 1)
			return (0);
	}
	data->source.sin_family = AF_INET;
	data->target.sin_family = AF_INET;
	return (1);
}

static int is_valid_mac(char *mac, uint8_t *mac_addr)
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
    d_dots = 0;
	while (d_dots < 6)
	{
		nb_bits = check_hex(tmp);
		if (nb_bits < 0x00 || nb_bits > 0xff)
			return (0);
        mac_addr[d_dots] = (uint8_t)nb_bits;
		while (*tmp && *tmp != ':')
			tmp++;
		tmp++;
        d_dots++;
	}
	return (1);
}

void    init_checks(int argc, char **argv, t_data *data)
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
    if (!is_valid_ip(argv, data))
		error("Invalid IP address", -1, TRUE);
	// Check if the user gave valid MAC addresses
	if (!is_valid_mac(argv[2], data->source_mac))
		error("Invalid source MAC address", -1, TRUE);
	if (!is_valid_mac(argv[4], data->target_mac))
		error("Invalid target MAC address", -1, TRUE);
}
