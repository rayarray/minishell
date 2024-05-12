/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_interpreter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 18:46:26 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <errno.h>
#include "interpreter.h"
#include "libft.h"

// n->end == -1 if the node contained quotes
static t_file_entry	*file_entry_from_node(t_node *n, t_mini_env *env)
{
	const char		*s;
	t_file_entry	*fe;

	s = n->str;
	if (s == NULL)
		return (NULL);
	if (n->typ == IN_A)
		fe = heredoc_parser(s, env, n->end != -1);
	else
		fe = new_file_entry(-1, n->typ, s);
	return (fe);
}

static t_file_entry	**file_entries_from_nodes(t_node *n, t_mini_env *env,
	int node_count)
{
	t_node			*n_iterator;
	t_file_entry	**entries;
	int				i;

	n_iterator = n;
	entries = ft_calloc(sizeof(t_file_entry *), node_count + 1);
	if (entries == NULL)
		print_strerror(errno, "File entries allocation fail\n");
	else
	{
		i = -1;
		while (++i < node_count)
		{
			entries[i] = file_entry_from_node(n_iterator, env);
			if (entries[i] == NULL)
			{
				free_fearray_and_unlink_temps(&entries);
				return (NULL);
			}
			n_iterator = n_iterator->nxt;
		}
		entries[i] = NULL;
	}
	return (entries);
}

static int	count_nodes(t_node *node)
{
	int	node_count;

	node_count = 0;
	while (node != NULL)
	{
		node_count++;
		node = node->nxt;
	}
	return (node_count);
}

// Initializes t_proc_hdr records recursively and adds file_entries to them.
// Returns 0 if succesful. Assumes that hdr has been allocated for as many
// records as there are t_cmdtabs
static int	fill_proc_hdrs(t_cmdtab *ct, t_proc_hdr *hdr, t_mini_env *env)
{
	int		redir_count;

	if (ct == NULL)
		return (0);
	init_process_header(hdr);
	hdr->cmd = copy_strarray((const char **)ct->cmd);
	if (hdr->cmd == NULL)
		return (1);
	redir_count = count_nodes(ct->redir);
	if (redir_count)
	{
		hdr->redirects = file_entries_from_nodes(ct->redir, env, redir_count);
		if (hdr->redirects == NULL)
		{
			free_strarray(&hdr->cmd);
			return (1);
		}
	}
	return (fill_proc_hdrs(ct->pipe, hdr + 1, env));
}

// Counts the number of commands in ct and then allocates and fills that many
// headers to *hdrs. Returns the number of commands or 0 on error.
int	get_headers(t_cmdtab *ct, t_proc_hdr **hdrs, t_mini_env *env)
{
	int			command_count;
	t_cmdtab	*ct_i;

	command_count = 0;
	ct_i = ct;
	while (ct_i != NULL)
	{
		command_count++;
		ct_i = ct_i->pipe;
	}
	*hdrs = ft_calloc(sizeof(t_proc_hdr), (command_count));
	if (*hdrs == NULL)
		return (print_strerror(errno, "Header allocation fail"));
	if (fill_proc_hdrs(ct, *hdrs, env))
	{
		free_proc_hdrs(hdrs, command_count);
		return (-2);
	}
	return (command_count);
}
