/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:36:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "libft.h"
#include "mini_builtins.h"
#include "minishell.h"

// Prints *string to stdout. if n is zero, prints a newline as well.
// Returns 0 on success, 1 on failure.
int	bi_echo(char **str)
{
	int		n;

	n = 0;
	if (str && *str && ft_strlen(*str) == 2 && ft_strncmp(*str, "-n", 2) == 0)
	{
		n = 1;
		++str;
	}
	while (str && *str != NULL)
	{
		if (write(STDOUT_FILENO, *str, ft_strlen(*str)) != ft_strlen(*str))
			return (print_strerror(errno, "echo"));
		if (*++str && write(STDOUT_FILENO, " ", 1) != 1)
			return (print_strerror(errno, "echo"));
	}
	if (!n && write(STDOUT_FILENO, "\n", 1) != 1)
		return (print_strerror(errno, "echo"));
	return (0);
}
