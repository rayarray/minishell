/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 15:16:29 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/20 21:27:14 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <limits.h>
#include "libft.h"
#include "minishell.h"
#include "mini_builtins.h"
#include "environ.h"

// Saves cwd as $PWD and saves previous $PWD as $OLDPWD. If getcwd fails,
// prints error and returns.
static int	set_pwd(t_strvec *env)
{
	char	pwd[PATH_MAX];
	char	oldpwd[PATH_MAX];

	if (!getcwd(pwd, PATH_MAX))
		return (print_strerror(errno,
				"cd: error retrieving current directory"));
	if (env_find(env, "PWD") != -1)
		ft_strlcpy(oldpwd, env_getval(env, "PWD"), PATH_MAX);
	env_add_kv(env, "PWD", pwd);
	env_add_kv(env, "OLDPWD", oldpwd);
	return (0);
}

static int	cdpath(t_strvec *env, char *dir)
{
	char	**cdpath;
	char	tentative_dir[PATH_MAX];
	int		i;

	if (ft_strcmp(dir, ".") || ft_strcmp(dir, "..") || ft_strcmp(dir, "/")
		|| ft_strncmp(dir, "./", 2) == 0 || ft_strncmp(dir, "../", 3) == 0)
		return (1);
	i = -1;
	cdpath = ft_split(env_search(env, "CDPATH", 0, -1), ':');
	while (cdpath && cdpath[++i] != NULL)
	{
		ft_strlcpy(tentative_dir, cdpath[i], PATH_MAX);
		ft_strlcat(tentative_dir, "/", PATH_MAX);
		ft_strlcat(tentative_dir, dir, PATH_MAX);
		if (chdir(tentative_dir) != 0)
			continue ;
		set_pwd(env);
		if (getcwd(tentative_dir, PATH_MAX))
			ft_printf("%s\n", tentative_dir);
		free_strarray(&cdpath);
		return (0);
	}
	free_strarray(&cdpath);
	return (1);
}

static int	go_home(t_strvec *env)
{
	char	wd[PATH_MAX];
	int		ret_val;

	ret_val = 0;
	if (env_exist_val(env, "HOME") <= 0)
		return (print_error(2, "cd: HOME not set", NULL));
	else if (env_exist_val(env, "HOME") == 1)
		wd[0] = '\0';
	else if (env_exist_val(env, "HOME") > 1)
		ft_strlcpy(wd, env_search(env, "HOME", 0, 4), PATH_MAX);
	if (wd[0] != '\0' && chdir(wd) && (print_strerror(errno, wd) * 0 + 1))
		ret_val = 1;
	return (ret_val);
}

// A valid argument is of PATH_MAX length or less.
// If given multiple arguments, prints error and returns.
// Differs from bash implementation in that 'cd ..' does not work if cwd is 
// inside a folder without access rights (chdir fails).
int	bi_cd(t_strvec *env, char **str)
{
	char	wd[PATH_MAX];

	if (str[0] != NULL && str[1] != NULL)
		return (print_error(2, "cd: too many arguments", NULL));
	if (str[0] == NULL)
		return (go_home(env));
	if (ft_strlen(str[0]) == 0)
		wd[0] = '\0';
	else if (str[0][0] == '/')
		ft_strlcpy(wd, str[0], PATH_MAX);
	else if (env_exist_val(env, "CDPATH") > 1 && cdpath(env, str[0]) == 0)
		return (0);
	else
		ft_strlcpy(wd, str[0], PATH_MAX);
	if (wd[0] != '\0' && chdir(wd) && (print_strerror(errno, str[0]) * 0 + 1))
		return (1);
	set_pwd(env);
	return (0);
}
