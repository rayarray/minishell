/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 01:20:02 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/07 21:36:29 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"
#include "mini_builtins.h"
#include "environ.h"

static int	eq_check(char *s)
{
	while (s && *s)
		if (*(s++) == '=')
			return (1);
	return (0);
}

// Prints environment variables and their content to stdout
int	bi_env(t_strvec *env)
{
	char	**e;
	int		length;
	int		err_check;

	e = env->vec;
	while (*e)
	{
		if (!eq_check(*e) && e++)
			continue ;
		length = ft_strlen(*e);
		err_check = write(STDOUT_FILENO, *e, length);
		if (err_check >= 0)
			err_check = ft_putchar_fd('\n', STDOUT_FILENO);
		if (err_check == 0)
			break ;
		e++;
	}
	if (err_check >= 0)
		return (0);
	perror(NULL);
	return (err_check);
}
