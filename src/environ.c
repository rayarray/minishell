/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 12:50:17 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/20 17:41:15 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include "libft.h"
#include "vector.h"
#include "mini_builtins.h"
#include "environ.h"
#include "minishell.h"

// initializes env vector, run before trying to access environment vars
// returns 0 if fail
// maybe todo later: SHELL, SHLVL variables
t_strvec	*env_init(void)
{
	t_strvec	*env;
	extern char	**environ;
	char		cwd[PATH_MAX];

	env = init_strvec(1, environ, 1);
	if (env == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Could not allocate env\n");
		return (NULL);
	}
	env->extra = 0;
	if (getcwd(cwd, PATH_MAX) == NULL)
		print_strerror(errno, "Could not get pwd");
	else if (env_add_kv(env, "PWD", cwd))
		ft_fprintf(STDERR_FILENO, "Could not add pwd\n");
	set_shlvl(env);
	return (env);
}

// checks existance of variable, returns -1 if no, index pos if yes
int	env_find(t_strvec *env, const char *s)
{
	int	i;
	int	j;

	i = 0;
	while (s && i < env->size)
	{
		j = 0;
		while (s[j] != '=' && s[j] && s[j] == env->vec[i][j])
			j++;
		if ((s[j] == 0 || s[j] == '=')
			&& (env->vec[i][j] == 0 || env->vec[i][j] == '='))
			return (i);
		i++;
	}
	return (-1);
}

// gets value of env of given env string, returns NULL if env not found or env
// doesn't have value. if nothing after = then returns just a pointer to 0
char	*env_getval(t_strvec *env, const char *s)
{
	char	*value;

	if (!s || *s == 0)
		return (NULL);
	if (*s == '?')
		return (exit_value_str(env));
	if (env_find(env, s) > -1)
	{
		value = env->vec[env_find(env, s)];
		while (value && *value)
			if (*(value++) == '=')
				return (value);
	}
	return (NULL);
}

// searches the environ for a variable
// full 0 returns only the variable content, 1 returns the full string
// if len > -1, variable name is len chars unless 0 or = comes first
char	*env_search(t_strvec *env, const char *s, int full, int len)
{
	char		**e;
	int			i[2];
	const char	*j;
	char		*k;

	if (!s || !*s || len == 0)
		return (NULL);
	i[0] = 0;
	e = env->vec;
	while (e[i[0]])
	{
		j = s;
		k = e[i[0]];
		i[1] = len;
		while (*j && *k && *k != '=' && *j == *k && i[1]-- && j++ && k++)
		{
			if (full == 0 && (*j == 0 || *j == '=' || i[1] == 0) && *k == '=')
				return (k + 1);
			else if (full && (*j == 0 || *j == '=' || i[1] == 0) && *k == '=')
				return (e[i[0]]);
		}
		i[0]++;
	}
	return (NULL);
}

// adds an environ variable. string given will be copied
// form is same as in export command (env=value)
int	env_add(t_strvec *env, char *s)
{
	int	i;

	i = env_find(env, s);
	if (i > -1)
	{
		free(env->vec[i]);
		env->vec[i] = ft_strdup(s);
	}
	else
		add_strvec(env, s, 1);
	return (0);
}
