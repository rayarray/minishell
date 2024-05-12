/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
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
#include "minishell.h"
#include "mini_builtins.h"
#include "environ.h"

// Prints the current working directory
// If current working directory cannot be accessed, will use the value of $PWD
// instead. If $PWD has not been set properly, might not print valid output.
int	bi_pwd(t_strvec *env)
{
	char	wd[PATH_MAX];
	char	*err_check;
	int		ret_val;

	ret_val = 0;
	err_check = getcwd(wd, PATH_MAX);
	if (err_check)
	{
		if (ft_printf(wd) <= 0 || ft_printf("\n") <= 0)
			ret_val = 0;
	}
	else
	{
		ret_val = errno;
		if (env_find(env, "$PWD"))
		{
			ft_printf("%s\n", env_search(env, "PWD", 0, -1));
			ret_val = 0;
		}
		else
			return (print_strerror(errno, "pwd"));
	}
	return (ret_val);
}
