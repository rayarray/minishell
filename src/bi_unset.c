/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:21:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 16:40:07 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "mini_builtins.h"
#include "environ.h"
#include "parser.h"

static int	chk_name(char *s)
{
	int		i;

	if (!s || *s == '\0')
		return (0);
	if ((!ft_isalpha(*s) && *s != '_') || *s == '=')
		return (0);
	s++;
	i = 1;
	while (*s)
	{
		if (!ft_isalnum(*s) && ft_findchrstr(*s, "_") == -1)
			return (0);
		s++;
		i++;
	}
	return (i);
}

// Unsets (deletes) environment variable(s)
int	bi_unset(t_strvec *env, char **str)
{
	int	ret;

	ret = 0;
	while (str && *str)
	{
		if (chk_name(*str) == 0)
		{
			write (STDERR_FILENO, "unset: `", 9);
			write (STDERR_FILENO, *str, ft_strlen(*str));
			write (STDERR_FILENO, "': not a valid identifier\n", 27);
			ret = 1;
		}
		else if (env_find(env, *str) > -1)
			rm_strvec(env, env->vec[env_find(env, *str)], 1, 1);
		str++;
	}
	return (ret);
}
