/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 13:24:59 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 16:50:57 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "interpreter.h"
#include "file_operations.h"
#include "minishell.h"
#include "mini_builtins.h"
#include "sig_handling.h"

static int	open_redirects(t_proc_hdr *hdr)
{
	t_file_entry	**f_i;
	int				err_check;

	hdr->stdin_redir = hdr->pipe[PIPE_READ];
	hdr->stdout_redir = hdr->pipe[PIPE_WRITE];
	if (!hdr->redirects)
		return (0);
	f_i = hdr->redirects;
	while ((*f_i) != NULL)
	{
		err_check = open_file_entry(*f_i);
		if (err_check)
			return (1);
		if ((*f_i)->mode == IN_A)
			unlink((*f_i)->filename);
		if ((*f_i)->mode == IN || (*f_i)->mode == IN_A)
			hdr->stdin_redir = (*f_i)->fd;
		else
			hdr->stdout_redir = (*f_i)->fd;
		++f_i;
	}
	return (0);
}

// i is exploiting the knowledge that 0 == stdin and 1 == stdout
static int	dup_redirects(t_proc_hdr *hdr)
{
	int	err_check;
	int	i;
	int	temp_stds[2];
	int	**redir_ptr;

	redir_ptr = (int *[2]){&hdr->stdin_redir, &hdr->stdout_redir};
	i = -1;
	while (++i < 2)
	{
		if (*redir_ptr[i] < 0)
			continue ;
		temp_stds[i] = dup(i);
		if (temp_stds[i] < 0)
			return (print_strerror(errno, hdr->cmd[0]));
		err_check = dup2(*redir_ptr[i], i);
		if (err_check < 0)
		{
			err_check = errno;
			close (temp_stds[i]);
			return (print_strerror(err_check, hdr->cmd[0]));
		}
		*redir_ptr[i] = temp_stds[i];
	}
	return (0);
}

static void	revert_dup_redirects(t_proc_hdr *hdr)
{
	if (hdr->stdout_redir >= 0)
	{
		dup2(hdr->stdout_redir, STDOUT_FILENO);
		close(hdr->stdout_redir);
	}
	if (hdr->stdin_redir >= 0)
	{
		dup2(hdr->stdin_redir, STDIN_FILENO);
		close(hdr->stdin_redir);
	}
}

// Executes the command in the header
int	exec_cmd(t_proc_hdr *hdr, t_mini_env *env)
{
	char	*exe;
	int		rv;

	rv = open_redirects(hdr);
	if (!rv)
	{
		rv = dup_redirects(hdr);
		if (*hdr->cmd && !rv && check_builtin(*hdr->cmd) == e_not_a_builtin)
		{
			exe = find_cmd(*hdr->cmd, env);
			rv = check_file_access(exe);
			if (!rv)
			{
				rv = execve(exe, hdr->cmd, env->env_vec->vec);
				rv = print_strerror(errno, exe);
			}
			free(exe);
		}
		else if (*hdr->cmd && !rv)
			rv = run_builtin(env->env_vec, check_builtin(*hdr->cmd), hdr->cmd);
		revert_dup_redirects(hdr);
	}
	return (rv);
}

// Starts the processes in headers.
int	start_processes(t_proc_hdr *hdrs, int hdr_c, t_mini_env *env)
{
	int		i;
	pid_t	pid;
	int		ret_val;

	i = -1;
	ret_val = 0;
	while (++i < hdr_c && !ret_val)
	{
		pid = fork();
		if (pid == 0)
		{
			default_signal(SIGINT);
			default_signal(SIGQUIT);
			close_pipes_backwards(&hdrs[i + 1], hdr_c - i - 2);
			ret_val = exec_cmd(&hdrs[i], env);
			free_proc_hdr(&hdrs[i]);
			exit (ret_val);
		}
		else if (pid < 0)
			return (print_strerror(errno, hdrs[i].cmd[0]));
		hdrs[i].pid = pid;
		ret_val = close_pipe(&hdrs[i]);
	}
	return (ret_val);
}
