/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_header.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 22:40:20 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:30:45 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "interpreter.h"
#include "minishell.h"

// Initializes an allocated process header
int	init_process_header(t_proc_hdr *ph)
{
	if (ph == NULL)
		return (-1);
	ph->pid = -1;
	ph->cmd = NULL;
	ph->stdin_redir = -1;
	ph->stdout_redir = -1;
	ph->redirects = NULL;
	ph->pipe[PIPE_READ] = -1;
	ph->pipe[PIPE_WRITE] = -1;
	ph->terminated = 0;
	return (0);
}

int	free_proc_hdrs(t_proc_hdr **hdrs, int header_count)
{
	int	i;

	i = -1;
	if (!hdrs)
		return (1);
	while (++i < header_count)
	{
		free_strarray(&(*hdrs)[i].cmd);
		free_fearray_and_unlink_temps(&(*hdrs)[i].redirects);
	}
	free(*hdrs);
	*hdrs = NULL;
	return (0);
}

int	free_proc_hdr(t_proc_hdr *hdr)
{
	if (!hdr)
		return (1);
	free_strarray(&hdr->cmd);
	free_fearray_and_unlink_temps(&hdr->redirects);
	return (0);
}
