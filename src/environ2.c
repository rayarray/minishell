/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:50:11 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/20 19:53:34 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include "libft.h"
#include "environ.h"
#include "minishell.h"

char	*exit_value_str(t_strvec *e)
{
	int	val;
	int	dig;

	val = e->extra;
	dig = 0;
	while (val > 0 && ++dig)
		val = val / 10;
	if (dig == 0)
		dig = 1;
	val = e->extra;
	e->extra_str[dig] = 0;
	while (dig--)
	{
		e->extra_str[dig] = '0' + val % 10;
		val /= 10;
	}
	return (&e->extra_str[0]);
}

// Adds a key-value pair to env.
int	env_add_kv(t_strvec *env, const char *key, const char *val)
{
	char	*s;
	int		ret_val;

	s = ft_strjoin2(key, val, '=');
	if (s == NULL)
		return (print_strerror(errno, key));
	ret_val = env_add(env, s);
	free(s);
	return (ret_val);
}

static int	is_shlvl_numval(const char *a)
{
	int	maxlen;
	int	i;

	maxlen = 3;
	i = 0;
	while (ft_isspace(*a))
		a++;
	if (!ft_isdigit(*a++))
		return (0);
	while (ft_isdigit(*a))
	{
		a++;
		i++;
		if (i == maxlen)
			return (print_error(0,
					"warning: shell level too high, resetting to 1", NULL));
	}
	while (ft_isspace(*a) && *a != '\0')
		a++;
	if (*a == '\0')
		return (1);
	return (0);
}

int	set_shlvl(t_strvec *env)
{
	char	*shlvl;
	int		shlvl_i;
	int		ret_val;

	ret_val = 0;
	shlvl = env_getval(env, "SHLVL");
	if (shlvl && is_shlvl_numval(shlvl))
	{
		shlvl_i = ft_atoi(shlvl) + 1;
		shlvl = ft_itoa(shlvl_i);
		if (shlvl != NULL)
		{
			ret_val = env_add_kv(env, "SHLVL", shlvl);
			free(shlvl);
			return (ret_val);
		}
	}
	ret_val = env_add_kv(env, "SHLVL", "1");
	return (ret_val);
}

// checks if env exists and if it has a value, -1 if env doesn't exist
// 0 if exists but doesn't have value and 1 if exists and has value but
// the value is just a null, 2 if exists and has value other than null
int	env_exist_val(t_strvec *env, const char *s)
{
	char	*str;

	if (env_find(env, s) == -1)
		return (-1);
	str = env->vec[env_find(env, s)];
	while (str && *str && *str != '=')
		str++;
	if (*str == 0)
		return (0);
	else if (*str == '=')
	{
		str++;
		if (*str)
			return (2);
		else if (!*str)
			return (1);
	}
	return (-2);
}
