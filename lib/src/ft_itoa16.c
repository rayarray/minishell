/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa16.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 20:24:29 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/24 20:12:54 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static unsigned int	ft_power(int a, unsigned int power)
{
	unsigned int	result;

	if (power == 0)
		return (1);
	result = a;
	while (power > 1)
	{
		result *= a;
		power--;
	}
	return (result);
}

static char	get_a_hex(unsigned int n, int cap)
{
	char	c;

	if (n < 10)
		return ('0' + n);
	else if (n < 16)
		c = 'a' + (n - 10);
	else
		c = 0;
	return (c - (cap * 32));
}

char	*ft_itoa16(unsigned int n, int cap)
{
	int				oom;
	unsigned int	i;
	char			*a;

	oom = 1;
	while (oom < 8 && n / ft_power(16, oom) != 0)
		oom++;
	a = malloc(sizeof(char) * (oom + 1));
	if (a == NULL)
		return (NULL);
	i = 0;
	while (oom > 0)
		a[i++] = get_a_hex((n / ft_power(16, --oom) % 16), cap);
	a[i] = '\0';
	return (a);
}
