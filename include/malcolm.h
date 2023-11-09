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

# include <sys/ioctl.h>

/* Ethernet ARP packet from RFC 826 */
typedef struct {
	uint16_t arp_hd;   /* Format of hardware address */
	uint16_t ptype;   /* Format of protocol address */
	uint8_t hlen;    /* Length of hardware address */
	uint8_t plen;    /* Length of protocol address */
	uint16_t op;    /* ARP opcode (command) */
	uint8_t sha[ETH_ALEN];  /* Sender hardware address */
	uint32_t spa;   /* Sender IP address */
	uint8_t tha[ETH_ALEN];  /* Target hardware address */
	uint32_t tpa;   /* Target IP address */
} arp_ether_ipv4;

typedef struct __attribute__((packed)) s_arp_header
{
	unsigned short arp_hd;
	unsigned short arp_pr;
	unsigned char arp_hdl;
	unsigned char arp_prl;
	unsigned short arp_op;
	unsigned char arp_sha[6];
	unsigned char arp_spa[4];
	unsigned char arp_dha[6];
	unsigned char arp_dpa[4];
}   t_arp_header;

typedef struct s_data
{
	struct sockaddr_in  source;
	int    source_decimal;
	struct sockaddr_in  target;
	int    target_decimal;
}   t_data;

extern int  g_packet_socket;

void    init_checks(int, char **, t_data *);
int     check_hex(char *);
void    parse(char **, t_data *);
void    poison(void *);
void    welcome();
void    error(const char *, int, int);

/*
 * BONUS
 */
char    *getMacAddress(const char *, int);

#endif //FT_MALCOLM_MALCOLM_H
