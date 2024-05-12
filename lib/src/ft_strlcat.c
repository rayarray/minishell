/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 22:01:11 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/24 20:14:32 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	unsigned int	original_dstlen;
	unsigned int	dstlen;
	int				i;

	if ((dst == NULL || src == NULL) && dstsize == 0)
		return (0);
	dstlen = 0;
	while (dst[dstlen] != '\0' && dstlen < dstsize)
		dstlen++;
	original_dstlen = dstlen;
	i = 0;
	while (src[i] != '\0')
	{
		if (dstlen + i + 1 < dstsize)
			dst[dstlen + i] = src[i];
		else if (dstlen + i + 1 == dstsize)
			dst[dstlen + i] = '\0';
		i++;
	}
	if (dstlen + i < dstsize)
		dst[dstlen + i] = '\0';
	return (original_dstlen + i);
}
