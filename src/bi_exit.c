/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 17:20:27 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 20:57:51 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "mini_builtins.h"
#include "libft.h"

static int	is_overmax(char *a)
{
	size_t	nbr;
	size_t	sign;
	int		digits;

	digits = 0;
	nbr = 0;
	sign = 0;
	while (ft_isspace(*a))
		a++;
	if (*a == '-')
		sign = 1;
	if (*a == '+' || *a == '-')
		a++;
	while (ft_isdigit(*a))
	{
		nbr *= 10;
		nbr += *a - '0';
		a++;
		digits++;
	}
	if (digits > 19 || nbr - sign > 9223372036854775807)
		return (1);
	return (0);
}

static char	is_numval(char *a)
{
	while (ft_isspace(*a))
		a++;
	if (*a == '-' || *a == '+')
		a++;
	if (!ft_isdigit(*a++))
		return (0);
	while (ft_isdigit(*a))
		a++;
	while (ft_isspace(*a) && *a != '\0')
		a++;
	if (*a == '\0')
		return (1);
	return (0);
}

int	bi_exit(t_strvec *env, char **args)
{
	char	*a;

	if (!args || !*args)
		exit (env->extra);
	a = args[0];
	if (!is_numval(a) || is_overmax(a))
	{
		ft_fprintf(STDERR_FILENO, "exit: %s: numeric argument required\n",
			args[0]);
		exit(255);
	}
	else if (args[1] != NULL)
	{
		ft_fprintf(STDERR_FILENO, "exit: too many arguments\n");
		exit(1);
	}
	exit((unsigned char)ft_atoi(a));
}
