/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:09:12 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 22:53:17 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "mini_builtins.h"
#include "libft.h"
#include "minishell.h"

t_builtin	check_builtin(char *s)
{
	if (ft_strcmp(s, BI_CD))
		return (e_cd);
	else if (ft_strcmp(s, BI_ECHO))
		return (e_echo);
	else if (ft_strcmp(s, BI_PWD))
		return (e_pwd);
	else if (ft_strcmp(s, BI_ENV))
		return (e_env);
	else if (ft_strcmp(s, BI_UNSET))
		return (e_unset);
	else if (ft_strcmp(s, BI_EXPORT))
		return (e_export);
	else if (ft_strcmp(s, BI_EXIT))
		return (e_exit);
	else
		return (e_not_a_builtin);
}

int	run_builtin(t_strvec *env, t_builtin t, char **args)
{
	if (!args && !args[0])
		return (-2);
	if (t == e_cd)
		return (bi_cd(env, &args[1]));
	else if (t == e_echo)
		return (bi_echo(&args[1]));
	else if (t == e_pwd)
		return (bi_pwd(env));
	else if (t == e_env)
		return (bi_env(env));
	else if (t == e_unset)
		return (bi_unset(env, &args[1]));
	else if (t == e_export)
		return (bi_export(env, &args[1]));
	else if (t == e_exit)
		bi_exit (env, &args[1]);
	ft_fprintf(STDERR_FILENO, "Unknown builtin\n");
	return (-1);
}
