//
// Created by tnoulens on 11/3/23.
//

#ifndef FT_MALCOLM_MALCOLM_H
# define FT_MALCOLM_MALCOLM_H

# ifndef BONUS
#  define BONUS 0
# endif

# define TRUE 1
# define FALSE 0
# define MAX_BUF 42

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <net/if.h>
# include <netinet/in.h>
# include <netdb.h>
# include <ifaddrs.h>
# include <linux/if_ether.h>
# include <signal.h>
# include <errno.h>
# include "colors.h"
# include "libft.h"
# include <netpacket/packet.h>
# include <net/ethernet.h>
# include <netinet/if_ether.h>
# include <sys/ioctl.h>

/* Ethernet ARP packet from RFC 826 */
typedef struct __attribute__((packed)) s_arp_packet
{
    // ARP header
	uint16_t arp_hd;            /* Format of hardware address */
	uint16_t arp_pr;            /* Format of protocol address */
	uint8_t arp_hdl;            /* Length of hardware address */
	uint8_t arp_prl;            /* Length of protocol address */
    uint16_t arp_opcode;        /* ARP opcode (command) */
    // ARP data
	uint8_t arp_sha[ETH_ALEN];  /* Sender hardware address */
	uint32_t arp_spa;           /* Sender IP address */
	uint8_t arp_dha[ETH_ALEN];  /* Target hardware address */
	uint32_t arp_dpa;           /* Target IP address */
}	t_arp_packet;

typedef enum e_type_of_attack
{
    REPLY = 0b00000001,
    REQUEST = 0b00000010,
    VERBOSE = 0b00000100,
}   t_type_of_attack;

typedef struct s_data
{
	struct sockaddr_in  source;
    uint8_t             source_mac[ETH_ALEN];
	struct sockaddr_in  target;
    uint8_t             target_mac[ETH_ALEN];
    int                 opt;
    char                interface[IFNAMSIZ];
    unsigned int        interface_index;
	uint8_t             my_mac[ETH_ALEN];
}	t_data;

extern int  g_packet_socket;

void	init_checks(int, char **, t_data *);
int		check_hex(char *);
void	poison(t_data *, t_arp_packet *, struct ethhdr *);
void	welcome();
void	error(const char *, int, int);
void    interface(t_data *);
void    build_arp_request(t_data *, t_arp_packet *);
void    get_link_layer_addr(t_data *, const unsigned char *, struct sockaddr_ll *);
int     print_data(t_arp_packet *, struct ethhdr *);
void    choose_socket_type(t_data *);
void    printMacAddress(const unsigned char *);
char    *netToStr(uint32_t);

#endif //FT_MALCOLM_MALCOLM_H
