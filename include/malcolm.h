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

extern int  g_packet_socket;

void    init_checks(int, char **);
int     check_hex(char *);
void	*parse(char **);
void	poison(void *);
void    welcome();
void    error(const char *, int, int);

/*
 * BONUS
 */
char    *getMacAddress(const char *, int);

#endif //FT_MALCOLM_MALCOLM_H
