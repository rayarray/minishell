/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_builtins.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:53:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 23:14:34 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_BUILTINS_H
# define MINI_BUILTINS_H
# include "vector.h"
# define BI_CD "cd"
# define BI_ECHO "echo"
# define BI_PWD "pwd"
# define BI_ENV "env"
# define BI_UNSET "unset"
# define BI_EXPORT "export"
# define BI_EXIT "exit"

// Typedefs to identify different builtin commands
typedef enum e_builtins {
	e_cd,
	e_echo,
	e_pwd,
	e_env,
	e_unset,
	e_export,
	e_exit,
	e_not_a_builtin
}	t_builtin;

// Functions for builtin commands
int			bi_echo(char **str);
int			bi_env(t_strvec *env);
int			bi_exit(t_strvec *env, char **args);
int			bi_pwd(t_strvec *env);
int			bi_unset(t_strvec *env, char **str);
int			bi_export(t_strvec *env, char **str);
int			bi_cd(t_strvec *env, char **str);

// Utility functions related to builtin commands
t_builtin	check_builtin(char *s);
int			run_builtin(t_strvec *env, t_builtin t, char **args);

#endif