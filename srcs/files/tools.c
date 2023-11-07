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
