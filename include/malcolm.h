//
// Created by tnoulens on 11/3/23.
//

#ifndef FT_MALCOLM_MALCOLM_H
# define FT_MALCOLM_MALCOLM_H

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
# include <signal.h>
# include <errno.h>
# include "colors.h"
# include "libft.h"

extern int  g_status;

void    init_checks(int, char **);
int     check_hex(char *);
void	parse(char **);
void	poison(void);

#endif //FT_MALCOLM_MALCOLM_H
