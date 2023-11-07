//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int check_hex(char *tmp)
{
	int     hex_value = -1;
	int     i = 0;
	char    *chunk;

	while (tmp[i] != ':' && tmp[i] != '\0')
	{
		if (!(tmp[i] >= '0' && tmp[i] <= '9')
			&& !(tmp[i] >= 'a' && tmp[i] <= 'f')
			&& !(tmp[i] >= 'A' && tmp[i] <= 'F'))
			return (-1);
		i++;
	}
	if (i > 2)
		return (-1);
	chunk = ft_substr(tmp, 0, i);
	hex_value = ft_atoi_base(chunk, 16);
	free(chunk);
	return (hex_value);
}

void    welcome()
{
	printf(CYAN"  __  _                       _           _\n");
	printf(" / _|| |_        _ __   __ _ | | __  ___ | | _ __\n");
	printf("|  _||  _|      | '  \\ / _` || |/ _|/ _ \\| || '  \\\n");
	printf("|_|   \\__|______|_|_|_|\\__/_||_|\\__|\\___/|_||_|_|_|\n");
	printf(GREEN"            A basic ARP poisoning program\n\n");
}

/*char    *getMacAddress(const char *interfaceName, int sockfd)
{
	struct ifreq ifr = {};
	char    mac[18];

	ft_memset(&ifr, 0, sizeof(ifr));
	ft_memset(mac, 0, sizeof(mac));
	snprintf(ifr.ifr_name, IFNAMSIZ, "%s", interfaceName);
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1)
		error("ioctl", errno);
	printf("My machine: MAC Address of %s: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       interfaceName,
	       (unsigned char)ifr.ifr_hwaddr.sa_data[0],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[1],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[2],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[3],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[4],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
	        (unsigned char)ifr.ifr_hwaddr.sa_data[0],
	        (unsigned char)ifr.ifr_hwaddr.sa_data[1],
	        (unsigned char)ifr.ifr_hwaddr.sa_data[2],
	        (unsigned char)ifr.ifr_hwaddr.sa_data[3],
	        (unsigned char)ifr.ifr_hwaddr.sa_data[4],
	        (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
	return (ft_strdup(mac));
}*/
