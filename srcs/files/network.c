//
// Created by tnoulens on 11/10/23.
//

#include "malcolm.h"

void    choose_socket_type(t_data *data)
{
	if (BONUS == TRUE && (data->opt & REQUEST))
	{
		// Using DGRAM builds the ethernet header for me, no need to build from scratch
		g_packet_socket = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
		if (g_packet_socket <= -1)
			error("socket() failed", errno, TRUE);
	}
	else
	{
		// Using RAW allow to receive raw packets
		g_packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
		if (g_packet_socket <= -1)
			error("socket() failed", errno, TRUE);
	}
}

void    interface(t_data *data)
{
	struct ifaddrs *ifap;
	struct ifaddrs *ifa;
	struct sockaddr_in *sa;
	char    *addr;

	getifaddrs(&ifap);
	for (ifa = ifap; ifa; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
		{
			sa = (struct sockaddr_in *)ifa->ifa_addr;
			addr = inet_ntoa(sa->sin_addr);
			printf("Interface: %s\n\tAddress: %s index: %d\n", ifa->ifa_name, addr, if_nametoindex(ifa->ifa_name));
            if (!ft_strncmp(ifa->ifa_name, "eth", 3) || !ft_strncmp(ifa->ifa_name, "enp", 3) || !ft_strncmp(ifa->ifa_name, "ens", 3))
            {
                data->interface_index = if_nametoindex(ifa->ifa_name);
                ft_memcpy(data->interface, ifa->ifa_name, ft_strlen(ifa->ifa_name) + 1);
            }
			if (ifa->ifa_broadaddr)
			{
				sa = (struct sockaddr_in *)ifa->ifa_broadaddr;
				addr = inet_ntoa(sa->sin_addr);
				printf("\tBroadcast: %s\n", addr);
			}
			else
				printf("\tNo broadcast address\n");
			if (BONUS == TRUE)
				getMacAddress(ifa->ifa_name, g_packet_socket);
		}
	}
	ft_putchar_fd('\n', 1);
	freeifaddrs(ifap);
}

void    getMacAddress(const char *interfaceName, int sockfd)
{
	struct ifreq ifr = {};
	char    mac[18];

	ft_memset(&ifr, 0, sizeof(ifr));
	ft_memset(mac, 0, sizeof(mac));
	snprintf(ifr.ifr_name, IFNAMSIZ, "%s", interfaceName);
	if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) <= -1)
		error("ioctl", errno, FALSE);
	printf("\tMAC Address of %s: %02X:%02X:%02X:%02X:%02X:%02X\n",
	       interfaceName,
	       (unsigned char)ifr.ifr_hwaddr.sa_data[0],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[1],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[2],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[3],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[4],
	       (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
}

void    getHost(t_data *data)
{
	char hostnameSource[NI_MAXHOST];
	char hostnameTarget[NI_MAXHOST];

    data->opt |= VERBOSE;
	interface(data);
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

void    get_link_layer_addr(t_data *data, const unsigned char *ether_broadcast_addr, struct sockaddr_ll *addr)
{
	(*addr).sll_family = AF_PACKET;
	(*addr).sll_halen = ETH_ALEN;
	(*addr).sll_protocol = htons(ETH_P_ARP);
	ft_memcpy((*addr).sll_addr, ether_broadcast_addr, ETH_ALEN);
	(*addr).sll_ifindex = (*data).interface_index;
}

void    build_arp_request(t_data *data, t_arp_packet *req)
{
	// make ARP header
	(*req).arp_hd = htons(ARPHRD_ETHER);
	(*req).arp_pr = htons(ETH_P_IP);
	(*req).arp_hdl = ETH_ALEN;
	(*req).arp_prl = sizeof(in_addr_t);
	(*req).arp_opcode = htons(ARPOP_REQUEST);
	// make ARP data
	ft_memcpy(&(*req).arp_dpa, &(*data).target.sin_addr, sizeof((*req).arp_dpa));
	ft_memcpy(&(*req).arp_dha, &(*data).target_mac, sizeof((*req).arp_dha));
	ft_memcpy(&(*req).arp_spa, &(*data).source.sin_addr, sizeof((*req).arp_spa));
	ft_memcpy(&(*req).arp_sha, &(*data).source_mac, sizeof((*req).arp_sha));
}

// This function achieve the same thing as the interface() function above but uses ioctl() instead of getifaddrs()
/*int getEthernetInterface(char ***interfaces)
{
	*//*
	 * Structure used in SIOCGIFCONF request.  Used to retrieve interface
	 * configuration for machine (useful for programs which must know all
	 * networks accessible).
	 *//*
	struct ifconf ifc;
	*//*
	 * Interface request structure used for socket ioctl's.  All interface
	 * ioctl's must have parameter definitions which begin with ifr_name.
	 * The remainder may be interface specific.
	 *//*
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
}*/
