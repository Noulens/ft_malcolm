//
// Created by tnoulens on 11/3/23.
//

#include "malcolm.h"

int check_hex(char *tmp)
{
	int hex_value = -1;
	int i = 0;
	char *chunk;

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
