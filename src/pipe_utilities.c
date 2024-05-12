/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utilities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:20:55 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 18:34:52 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include "interpreter.h"
#include "minishell.h"

// Attempts to close pipes of a t_proc_hdr.
int	close_pipe(t_proc_hdr *header)
{
	int	ret_val;

	ret_val = 0;
	if (header->pipe[PIPE_READ] >= 0 && close(header->pipe[PIPE_READ]))
		ret_val = print_strerror(errno, header->cmd[0]);
	if (header->pipe[PIPE_WRITE] >= 0 && close(header->pipe[PIPE_WRITE]))
		ret_val = print_strerror(errno, header->cmd[0]);
	return (ret_val);
}

// Closes pipes of header_count t_proc_hdr's in an array, counting backwards
void	close_pipes_backwards(t_proc_hdr *headers, int i)
{
	while (i >= 0)
	{
		close_pipe(&headers[i]);
		headers[i].pipe[PIPE_READ] = -1;
		headers[i].pipe[PIPE_WRITE] = -1;
		i--;
	}
}

// Opens pipes to be used for data transfers between processes
int	open_pipes(t_proc_hdr *headers, int header_count)
{
	int	i;
	int	ret_val;

	i = 0;
	while (++i < header_count)
	{
		ret_val = pipe(headers[i].pipe);
		if (ret_val)
		{
			ret_val = print_strerror(errno, "fork");
			close_pipes_backwards(headers, i - 1);
			return (ret_val);
		}
		headers[i - 1].pipe[PIPE_WRITE] = headers[i].pipe[PIPE_WRITE];
		headers[i].pipe[PIPE_WRITE] = -1;
	}
	return (0);
}
