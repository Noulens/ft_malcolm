/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ld.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 17:37:34 by tnoulens          #+#    #+#             */
/*   Updated: 2022/10/04 15:18:57 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_ld(int nbr)
{
	char	*p;
	int		j;

	p = ft_ltoa(nbr);
	j = ft_strlen(p);
	ft_putstr_fd(p, 1);
	if (p)
		free(p);
	return (j);
}