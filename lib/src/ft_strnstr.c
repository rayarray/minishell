/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 20:16:26 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/24 20:14:49 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// Looks up to len characters for needle in haystack and returns a pointer to
// the start of it. Returns NULL if not found.
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	while (i < len && haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			j = i;
			while (++j < len && needle[j - i] != '\0'
				&& haystack[j] == needle[j - i]);
			if (needle[j - i] == '\0')
				return ((char *)&haystack[i]);
			else if (j == len)
				return (NULL);
		}
		i++;
	}
	return (NULL);
}
