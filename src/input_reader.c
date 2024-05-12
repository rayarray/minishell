/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:48:29 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft.h"
#include "sig_handling.h"
#include "input_reader.h"

// Returns number of characters written to buffer or <0 if there was an error
// when closing src.
static int	read_and_close(char *dst, int src, int dst_size)
{
	int	read_count;
	int	char_count;

	char_count = 0;
	read_count = 1;
	while (read_count > 0 && char_count < dst_size)
	{
		read_count = read(src, dst, dst_size - char_count);
		if (read_count < 0)
		{
			print_strerror(errno, "Reading input pipe");
			break ;
		}
		char_count += read_count;
		dst += read_count;
	}
	if (close(src))
		return (print_strerror(errno, "Closing input readpipe"));
	return (char_count);
}

static int	write_close_free(char *src, int dst, int src_size)
{
	int		write_count;
	int		char_count;
	char	*src_iter;

	char_count = 0;
	write_count = 1;
	src_iter = src;
	while (write_count > 0 && char_count < src_size)
	{
		write_count = write(dst, src_iter, src_size - char_count);
		if (write_count < 0)
		{
			print_strerror(errno, "Writing input pipe");
			break ;
		}
		char_count += write_count;
		src_iter += write_count;
	}
	free(src);
	if (close(dst))
		return (print_strerror(errno, "Closing input writepipe"));
	return (char_count);
}

static void	query(int pip[2])
{
	char	*s[3];

	default_signal(SIGINT);
	if (close(pip[PIPE_READ]))
		exit(print_strerror(errno, "Query pipe_read"));
	s[0] = readline(PROMPT);
	while (s[0] && check_for_open_quotes_or_pipeline(s[0]))
	{
		add_history(s[0]);
		s[1] = readline(QUOTE_PROMPT);
		if (s[1] == NULL)
		{
			ft_fprintf(STDERR_FILENO, "Syntax error: unexpected end of file\n");
			break ;
		}
		s[2] = ft_strjoin2(s[0], s[1], '\n');
		free(s[1]);
		free(s[0]);
		s[0] = s[2];
	}
	if (s[0] && write_close_free(s[0], pip[PIPE_WRITE], ft_strlen(s[0]) + 1))
		exit (1);
	exit(0);
}

// Returns 1 if child was killed by SIGINT
static int	wait_for_child(pid_t pid, char *str)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		*str = 3;
		*(str + 1) = '\0';
		ft_putchar_fd('\n', STDOUT_FILENO);
		return (1);
	}
	return (0);
}

int	get_input(char **input, int input_size)
{
	pid_t	pid;
	int		pip[2];
	int		err_check;

	if (pipe(pip))
		return (print_strerror(errno, "Input pipe"));
	pid = fork();
	if (pid == 0)
		query(pip);
	if (pid < 0)
	{
		err_check = errno;
		close(pip[PIPE_READ]);
		close(pip[PIPE_WRITE]);
		return (print_strerror(err_check, "Input fork"));
	}
	if (close(pip[PIPE_WRITE]))
		return (print_strerror(errno, "Output pipe_read"));
	err_check = read_and_close(*input, pip[PIPE_READ], input_size);
	if (!wait_for_child(pid, *input) && err_check <= 0)
		return (1);
	if (!is_empty(*input))
		add_history(*input);
	return (0);
}
