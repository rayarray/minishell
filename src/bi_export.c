/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 01:00:04 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/20 22:02:53 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "mini_builtins.h"
#include "environ.h"
#include "parser.h"

void	print_declarelist(t_strvec *env, char *str)
{
	int		i;

	if (str)
		return ;
	i = 0;
	while (env->vec[i])
	{
		write (STDOUT_FILENO, "declare -x ", 11);
		str = env->vec[i];
		while (*str && *str != '=')
			write (STDOUT_FILENO, str++, 1);
		if (*str == '=' && str++)
		{
			write (STDOUT_FILENO, "=\"", 2);
			while (*(str) != 0)
				write(STDOUT_FILENO, str++, 1);
			write (STDOUT_FILENO, "\"", 1);
		}
		write (STDOUT_FILENO, "\n", 1);
		i++;
	}
}

static int	chk_name(char *s)
{
	int	i;

	if (!s)
		return (0);
	if ((!ft_isalpha(*s) && *s != '_') || *s == '=')
		return (0);
	s++;
	i = 1;
	while (*s && *s != '=')
	{
		if (!ft_isalnum(*s) && ft_findchrstr(*s, "_") == -1)
			return (0);
		s++;
		i++;
	}
	return (i);
}

// Prints environment variables and their content to stdout
int	bi_export(t_strvec *env, char **str)
{
	int		ret;

	if (!str)
		return (2);
	print_declarelist(env, *str);
	ret = 0;
	while (str && *str)
	{
		if (chk_name(*str) == 0)
		{
			write(STDERR_FILENO, "export: `", 9);
			write(STDERR_FILENO, *str, ft_strlen(*str));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			ret = 1;
		}
		else
			env_add(env, *str);
		str++;
	}
	return (ret);
}
