//
// Created by tnoulens on 11/10/23.
//

#include "malcolm.h"

char    *getMacAddress(const char *interfaceName, int sockfd)
{
	struct ifreq ifr = {};
	char    mac[18];
	char    *tmp;

	ft_memset(&ifr, 0, sizeof(ifr));
	ft_memset(mac, 0, sizeof(mac));
	snprintf(ifr.ifr_name, IFNAMSIZ, "%s", interfaceName);
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) <= -1)
		error("ioctl", errno, FALSE);
	printf("MAC Address of %s: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       interfaceName,
	       (unsigned char)ifr.ifr_hwaddr.sa_data[0],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[1],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[2],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[3],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[4],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
//	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X",
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[0],
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[1],
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[2],
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[3],
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[4],
//	        (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
//	tmp = ft_strdup(mac);
//	if (!tmp)
//		error("enomem", errno, TRUE);
//	return (tmp);
	(void)tmp;
	return (NULL);
}

int getEthernetInterface(char ***interfaces)
{
	/*
	 * Structure used in SIOCGIFCONF request.  Used to retrieve interface
	 * configuration for machine (useful for programs which must know all
	 * networks accessible).
	 */
	struct ifconf ifc;
	/*
	 * Interface request structure used for socket ioctl's.  All interface
	 * ioctl's must have parameter definitions which begin with ifr_name.
	 * The remainder may be interface specific.
	 */
	struct ifreq ifr[10]; // 10 interface max
	size_t i;
	unsigned long ifc_num;

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_buf = (caddr_t)ifr; // the ifc buffer points to the ifr table
	// the ioctl call fills the ifc structure that is accessible through the ifr table
	if (ioctl(g_packet_socket, SIOCGIFCONF, &ifc) == 0)
	{
		ifc_num = ifc.ifc_len / sizeof(struct ifreq);
		printf("%lu interfaces found\n", ifc_num);
		// collect the interfaces names
		*interfaces = malloc(sizeof(char *) * (ifc_num + 1));
		if (!(*interfaces))
			error("enomem", errno, TRUE);
		(*interfaces)[ifc_num] = NULL;
		for (i = 0; i < ifc_num; ++i)
		{
			if (ifr[i].ifr_addr.sa_family != AF_INET)
			{
				continue;
			}
			printf("interface: %s\n", ifr[i].ifr_name);
			if (ioctl(g_packet_socket, SIOCGIFADDR, &ifr[i]) == 0)
				printf("\taddress: %s\n", inet_ntoa(((struct sockaddr_in *)(&ifr[i].ifr_addr))->sin_addr));
			else
				error("ioctl", errno, FALSE);
			if (ioctl(g_packet_socket, SIOCGIFBRDADDR, &ifr[i]) == 0)
				printf("\tbroadcast: %s\n", inet_ntoa(((struct sockaddr_in *)(&ifr[i].ifr_broadaddr))->sin_addr));
			else
				error("ioctl", errno, FALSE);
			if (ioctl(g_packet_socket, SIOCGIFNETMASK, &ifr[i]) == 0)
				printf("\tnetmask: %s\n", inet_ntoa(((struct sockaddr_in *)(&ifr[i].ifr_netmask))->sin_addr));
			else
				error("ioctl", errno, FALSE);
			(*interfaces)[i] = ft_strdup(ifr[i].ifr_name);
			if (!((*interfaces)[i]))
			{
				while (--i)
					free((*interfaces)[i]);
				free(*interfaces);
				error("enomem", errno, TRUE);
			}
		}
	}
	else
	{
		error("ioctl", errno, FALSE);
		return (1);
	}
	ft_putchar_fd('\n', 1);
	return (0);
}

void verbose(t_data *data)
{
	char **interfaces = NULL;

	if (getEthernetInterface(&interfaces) != 0)
		error("getEthernetInterface() failed", errno, FALSE);
	for (int i = 0; interfaces[i] != NULL; i++)
		getMacAddress(interfaces[i], g_packet_socket);
	ft_free_split(interfaces);
	ft_putchar_fd('\n', 1);
	char hostnameSource[NI_MAXHOST];
	char hostnameTarget[NI_MAXHOST];
	int codeS = getnameinfo((const struct sockaddr *)(&(*data).source), sizeof((*data).source), hostnameSource, NI_MAXHOST, NULL, 0, 0);
	if (codeS != 0)
	{
		fprintf(stderr, "getnameinfo failed: %s\n", gai_strerror(codeS));
		ft_memcpy(hostnameSource, "Unknown", 8);
	}
	printf("Hostname of source: %s\n", hostnameSource);
	int codeT = getnameinfo((const struct sockaddr *)(&(*data).target), sizeof((*data).target), hostnameTarget, NI_MAXHOST, NULL, 0, 0);
	if (codeT != 0)
	{
		fprintf(stderr, "getnameinfo failed: %s\n", gai_strerror(codeT));
		ft_memcpy(hostnameTarget, "Unknown", 8);
	}
	printf("Hostname of target: %s\n", hostnameTarget);
}
