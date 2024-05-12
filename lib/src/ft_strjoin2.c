/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:02:34 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/24 20:14:29 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

// Modified slightly from the original definition.
// Optional separator parameter to add between the two strings. If it is not
// needed, the value '\0' should be passed as separator.
char	*ft_strjoin2(char const *s1, char const *s2, char separator)
{
	int		length;
	int		s1_len;
	int		s2_len;
	char	*s3;

	s1_len = 0;
	s2_len = 0;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 != NULL)
		s1_len = ft_strlen(s1);
	if (s2 != NULL)
		s2_len = ft_strlen(s2);
	length = s1_len + s2_len + (separator != '\0');
	s3 = ft_calloc(sizeof(char), (length + 1));
	if (s3 == NULL)
		return (NULL);
	if (s1_len > 0)
		ft_strlcpy(s3, s1, s1_len + 1 + (separator != '\0'));
	if (separator)
		s3[s1_len] = separator;
	s3[s1_len + 1] = '\0';
	if (s2_len > 0)
		ft_strlcat(s3, s2, length + 1);
	return (s3);
}
