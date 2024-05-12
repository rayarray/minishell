/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_reader_utilities.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:29:35 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 22:20:31 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input_reader.h"
#include "libft.h"

int	is_empty(char *s)
{
	while (s && *s != '\0')
	{
		if (! ft_isspace(*s++))
			return (0);
	}
	return (1);
}

// Returns 1 if open quote or pipeline is found. Otherwise 0.
int	check_for_open_quotes_or_pipeline(char *s)
{
	char	open_quote_found;
	char	*pipe_ptr;

	open_quote_found = '\0';
	pipe_ptr = NULL;
	while (*s != '\0')
	{
		if ((*s == '\'' || *s == '"') && !open_quote_found)
			open_quote_found = *s;
		else if (*s == open_quote_found)
			open_quote_found = 0;
		else if (*s == '|')
			pipe_ptr = s;
		s++;
	}
	if (open_quote_found || (pipe_ptr && is_empty(pipe_ptr + 1)))
		return (1);
	return (0);
}
