/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 21:28:56 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/01 16:03:26 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0 || src == NULL)
		return (0);
	while (src[i] != '\0')
	{
		if (i < dstsize - 1)
			dst[i] = src[i];
		else if (i == dstsize - 1)
			dst[i] = '\0';
		i++;
	}
	if (i < dstsize)
		dst[i] = '\0';
	return (i);
}
