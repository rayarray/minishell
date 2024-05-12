/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 18:23:36 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 19:29:43 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "minishell.h"
#include "interpreter.h"
#include "file_operations.h"
#include "libft.h"
#include "sig_handling.h"
#include "heredoc.h"

static char	*join_nodes(t_heredoc_node	*list)
{
	int		i;
	char	*output;

	output = NULL;
	output = malloc(sizeof(char) * (get_total_length(list)));
	if (output == NULL)
	{
		perror ("heredoc: join_nodes: Malloc failed");
		return (NULL);
	}
	i = 0;
	while (list->typ != LAST_NODE)
	{
		while (--list->end > 0)
			output[i++] = *(list->str++);
		list++;
	}
	output[i] = '\0';
	return (output);
}

// Takes environment and pointer to a variable name (key), searches for the
// value of the variable and allocates a new t_heredoc_node object to store the
// value and its length in. If no variable of such name was found, creates a 
// node with NULL string. Returns the length of the key.
static void	expand_variables(t_mini_env *env, const char *str,
	t_heredoc_node *list)
{
	int		i;
	int		name_len;
	int		char_count;

	i = -1;
	char_count = 0;
	list->str = str;
	while (str[++i] != '\0')
	{
		list->end = ++char_count;
		if (str[i] != '$' || !is_var_name_char(str[i + 1], 0))
			continue ;
		char_count = 0;
		name_len = get_var_name_length(&str[i + 1]);
		(++list)->str = env_search(env->env_vec, &str[i + 1], 0, name_len);
		i += name_len;
		if (list->str != NULL && list->str[0] != '\0')
		{
			list->end = ft_strlen(list->str) + 1;
			list++;
		}
		list->str = &str[i + 1];
	}
	list->end = ++char_count;
	*(++list) = (t_heredoc_node){0, 0, LAST_NODE};
}

// Expands variables in str. Replaces *str with a new string if it contains
// variables to expand. In case of error, *str is unaltered and returns a value
// different from 0.
static int	heredoc_expand(t_mini_env *env, char **str)
{
	t_heredoc_node	*list;
	int				var_count;
	char			*exp_s;

	var_count = count_vars(*str);
	if (var_count == 0)
		return (0);
	list = ft_calloc(sizeof(t_heredoc_node), (2 * var_count + 2));
	if (list == NULL)
		return (print_strerror(errno, "heredoc_expand: Could not init nodes"));
	expand_variables(env, *str, list);
	exp_s = join_nodes(list);
	free(list);
	if (exp_s == NULL)
		return (1);
	free(*str);
	*str = (exp_s);
	return (0);
}

static pid_t	heredoc_read(const char *lim, t_mini_env *env,
	t_file_entry *rand_file, int var_exp)
{
	char	*str;

	default_signal(SIGINT);
	while (1)
	{
		str = readline(HEREDOC_PROMPT);
		if (str == NULL)
		{
			ft_fprintf(STDERR_FILENO, "warning: here-document delimited by"\
				" end-of-file (wanted `%s')\n", lim);
			exit(0);
		}
		if (ft_strlen(lim) == ft_strlen(str)
			&& ft_strncmp(lim, str, ft_strlen(lim)) == 0)
			break ;
		if (var_exp)
			heredoc_expand(env, &str);
		if (write_string_and_newline(rand_file->fd, str))
			break ;
		free(str);
	}
	free(str);
	exit(0);
}

// Creates a file in TMPDIR, /tmp or cwd, reads stdin until encounters lim on a
// new line writes the input into the file and returns the associated
// t_file_entry or NULL on error.
t_file_entry	*heredoc_parser(const char *lim, t_mini_env *env, int var_exp)
{
	t_file_entry	*rand_file;
	pid_t			pid;
	int				status;

	rand_file = open_random_file(env);
	if (rand_file == NULL)
		return (NULL);
	pid = fork();
	if (pid == 0)
		heredoc_read(lim, env, rand_file, var_exp);
	else if (pid > 0)
	{
		pid = wait(&status);
		close_file_entry(rand_file);
		if (!WIFSIGNALED(status) && WEXITSTATUS(status) == 0)
			return (rand_file);
		else if (WIFSIGNALED(status))
			ft_putchar_fd('\n', STDERR_FILENO);
	}
	else
		print_strerror(errno, "heredoc_parser: Fork failed");
	close(rand_file->fd);
	unlink(rand_file->filename);
	free_file_entry(&rand_file);
	return (NULL);
}
