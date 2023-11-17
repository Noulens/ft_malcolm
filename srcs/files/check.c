//
// Created by tnoulens on 11/6/23.
//

#include "malcolm.h"

char    *hostnameResolution(const char *hostname)
{
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *r;
	int status;
	char buffer[INET_ADDRSTRLEN];
	char *ret;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; // TCP
	status = getaddrinfo(hostname, 0, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return (NULL);
	}
	r = res;
	while (r != NULL)
	{
		if (r->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)r->ai_addr;
			inet_ntop(r->ai_family, &(ipv4->sin_addr), buffer, sizeof buffer);
			printf("IP adresses for %s: %s\n", hostname, buffer);
		}
		r = r->ai_next;
	}
	freeaddrinfo(res);
	ret = ft_strdup(buffer);
	return (ret);
}

static int is_valid_ip(char **ip, t_data *data)
{
	char    *source = ip[1];
	char    *target = ip[3];
	if (BONUS == TRUE)
	{
		for (int i = 0; ip[1][i]; i++)
		{
			if (ft_isalpha(ip[1][i]))
			{
				source = hostnameResolution(ip[1]);
				if (!source)
					return (0);
				break ;
			}
			if (ip[1][i + 1] == '\0')
				source = ft_strdup(ip[1]);
		}
		for (int i = 0; ip[3][i]; i++)
		{
			if (ft_isalpha(ip[3][i]))
			{
				target = hostnameResolution(ip[3]);
				if (!target)
					return (free(source), 0);
				break ;
			}
			if (ip[3][i + 1] == '\0')
				target = ft_strdup(ip[3]);
		}
		if (ft_strnstr(source, ".", ft_strlen(source)) == NULL)
		{
			char    *test = ft_itoa(ft_atoi(source));
			if (!ft_strcmp(test, source))
			{
				data->source.sin_family = AF_INET;
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
		free(source);
		if (ft_strnstr(target, ".", ft_strlen(target)) == NULL)
		{
			char    *test = ft_itoa(ft_atoi(target));
			if (!ft_strcmp(test, target))
			{
				data->target.sin_family = AF_INET;
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
		free(target);
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
	if (getuid() != 0)
		error("You must be root to run this program", -1, TRUE);
	if (BONUS == TRUE)
	{
		if ((argc != 6 && argc != 7) || (ft_strcmp(argv[5], "--request") && ft_strcmp(argv[5], "--reply")) || (argc == 7 && ft_strcmp(argv[6], "--verbose") != 0))
			error("Usage: ./ft_malcolm <source ip> <source MAC addr> <target ip> <target MAC addr> <Type of attack> [--verbose]", -1, TRUE);
        if (!ft_strcmp(argv[5], "--request"))
            data->opt |= REQUEST;
        else
            data->opt |= REPLY;
		if (argc == 7)
			data->opt |= VERBOSE;
	}
	else if (argc != 5)
	    error("Usage: ./ft_malcolm <source ip> <source MAC addr> <target ip> <target MAC addr>", -1, TRUE);
    if (!is_valid_ip(argv, data))
		error("Invalid IP address", -1, TRUE);
	if (!is_valid_mac(argv[2], data->source_mac))
		error("Invalid source MAC address", -1, TRUE);
	if (!is_valid_mac(argv[4], data->target_mac))
		error("Invalid target MAC address", -1, TRUE);
}
