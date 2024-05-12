/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:04:40 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include "heredoc.h"
#include "libft.h"
#include "minishell.h"

// Checks if a character is valid to be a part of variable name at given index
int	is_var_name_char(char c, int index)
{
	int		result;

	result = 0;
	if (c != '\0')
	{
		if (index == 0 && ft_strchr(ENV_VAR_1_CHARS, c))
			result = 1;
		else if (index > 0 && ft_strchr(ENV_VAR_N_CHARS, c))
			result = 1;
	}
	return (result);
}

// Counts number of environment variable names in a string
int	count_vars(const char *str)
{
	int	c;

	c = 0;
	while (*str != '\0')
	{
		if (*str == '$' && is_var_name_char(*(str + 1), 0))
		{
			c++;
			str += 2;
		}
		else
			str++;
	}
	return (c);
}

// Returns the length of a variable name pointed to by s
int	get_var_name_length(const char *s)
{
	int	i;

	i = 0;
	while (is_var_name_char(s[i], i))
		i++;
	return (i);
}

// Get total length of all strings in a list of nodes
int	get_total_length(t_heredoc_node *list)
{
	int	len;

	len = 0;
	while (list->typ != LAST_NODE)
	{
		len += list->end;
		list++;
	}
	return (len);
}

int	write_string_and_newline(int fd, char *str)
{
	int	strlen;
	int	wrlen;

	strlen = ft_strlen(str);
	wrlen = write(fd, str, strlen);
	while (wrlen != strlen)
	{
		if (wrlen < 0)
			return (print_strerror(errno, "write_string_and_newline"));
		str += wrlen;
		strlen -= wrlen;
		wrlen = write(fd, str, strlen);
	}
	wrlen = write(fd, "\n", 2);
	if (wrlen < 2)
		return (print_strerror(errno, "write_string_and_newline"));
	return (0);
}
