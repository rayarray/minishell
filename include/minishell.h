/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:36:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 16:42:35 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <limits.h>
# include "vector.h"
# define PROMPT "minishell> "
# define QUOTE_PROMPT "> "
# define HEREDOC_PROMPT "> "
# define MINI_ARG_MAX 262144
# define PIPE_READ 0
# define PIPE_WRITE 1
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

// Struct for holding environment variables. Kind of redundant
typedef struct s_mini_env
{
	t_strvec	*env_vec;
}	t_mini_env;

// minishell_utils
int		print_strerror(int errnum, const char *s);
int		print_error(int errnum, const char *s, const char *suffix);
char	**copy_strarray(const char **src);
char	**free_strarray(char ***array);
int		ft_strcmp(const char *s1, const char *s2);

#endif