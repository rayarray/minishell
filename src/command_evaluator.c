/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_evaluator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 18:14:30 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 17:18:05 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "mini_builtins.h"
#include "interpreter.h"
#include "parser.h"
#include "libft.h"
#include "sig_handling.h"

static inline t_proc_hdr	*get_hdr_by_pid(pid_t pid, t_proc_hdr *hdrs,
	int hdr_count)
{
	int	i;

	i = -1;
	while (++i < hdr_count)
	{
		if (hdrs[i].pid == pid)
			return (&hdrs[i]);
	}
	return (NULL);
}

// Sends SIGINT to the processes recorded in hdrs unless they have been 
// terminated.
static int	filicide(t_proc_hdr *hdrs, int hdr_c, int termsig)
{
	int	i;

	if (termsig != SIGQUIT && termsig != SIGINT)
		return (0);
	if (termsig == SIGQUIT)
		ft_fprintf(STDERR_FILENO, "Quit: 3");
	ft_fprintf(STDERR_FILENO, "\n");
	i = -1;
	while (++i < hdr_c)
	{
		if (hdrs[i].terminated == 0)
			kill(hdrs[i].pid, SIGINT);
	}
	return (1);
}

// wait for processes to finish and returns the exit code of the rightmost one.
static int	wait_for_processes_to_finish(t_proc_hdr *hdrs, int header_count)
{
	pid_t		pid;
	int			status;
	int			last_status;
	int			filicided;
	t_proc_hdr	*hdr;

	filicided = 0;
	pid = wait(&status);
	while (pid >= 0)
	{
		hdr = get_hdr_by_pid(pid, hdrs, header_count);
		hdr->terminated = 1;
		if (WIFSIGNALED(status) && (hdr->pid) == hdrs[header_count - 1].pid)
			last_status = 128 + WTERMSIG(status);
		else if ((hdr->pid) == hdrs[header_count - 1].pid)
			last_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && !filicided)
			filicided = filicide(hdrs, header_count, WTERMSIG(status));
		pid = wait(&status);
	}
	return (last_status);
}

// Evaluates the command in ct and returns the value. Frees ct in the process.
int	evaluate(t_cmdtab **ct, t_mini_env *env)
{
	int			rv;
	t_proc_hdr	*headers;
	int			hdr_c;

	hdr_c = get_headers(*ct, &headers, env);
	if (hdr_c <= 0)
		return (hdr_c);
	rv = open_pipes(headers, hdr_c);
	enable_echoctl();
	if (!rv && hdr_c == 1 && check_builtin(headers->cmd[0]) != e_not_a_builtin)
		rv = exec_cmd(headers, env);
	else if (! rv)
	{
		rv = start_processes(headers, hdr_c, env);
		if (! rv)
			rv = wait_for_processes_to_finish(headers, hdr_c);
	}
	disable_echoctl();
	free_proc_hdrs(&headers, hdr_c);
	return (rv);
}
