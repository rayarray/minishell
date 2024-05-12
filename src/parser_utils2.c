/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 09:58:16 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/14 22:31:47 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "parser.h"
#include "environ.h"
#include "libft.h"

int	ft_findchrstr(char c, char *s)
{
	int	i;

	i = 1;
	while (c && s && *s)
	{
		if (c == *s)
			return (i);
		s++;
		i++;
	}
	return (-1);
}

// returns !0 if valid char for env variable in index i (starting from 0)
int	isenvchar(char c, int i)
{
	if (i == 0)
		return (ft_isalpha(c) || c == '_' || c == '?');
	return (ft_isalnum(c) || c == '_');
}

// frees a t_cmdtab
void	destroy_cmdtab(t_cmdtab *ct)
{
	t_node	*n;

	while (ct->_cmd)
	{
		n = ct->_cmd;
		ct->_cmd = n->nxt;
		free((char *)n->str);
		free(n);
	}
	while (ct->redir)
	{
		n = ct->redir;
		ct->redir = n->nxt;
		free((char *)n->str);
		free(n);
	}
	free(ct->cmd);
	if (ct->error)
		free(ct->error);
	if (ct->pipe)
		destroy_cmdtab(ct->pipe);
	free (ct);
}

t_node	*list_rm_emptynodes(t_node *l, t_node *n)
{
	t_node	*nxt;

	while (l && l->end != -1 && (!l->str || l->end == 0 || *l->str == 0))
	{
		n = l;
		l = l->nxt;
		free ((void *)n->str);
		free (n);
	}
	n = l;
	if (n)
		nxt = n->nxt;
	while (n && nxt)
	{
		if (nxt->end != -1 && (!nxt->str || nxt->end == 0 || *nxt->str == 0))
		{
			n->nxt = nxt->nxt;
			free ((void *)nxt->str);
			free (nxt);
		}
		else
			n = n->nxt;
		nxt = n->nxt;
	}
	return (l);
}

// removes and frees any empty nodes in _cmd and returns the number of nodes
// in _cmd after removing. does not remove empty nodes from redir list
int	destroy_emptynodes(t_cmdtab *t)
{
	t_node	*n;
	int		i;

	t->_cmd = list_rm_emptynodes(t->_cmd, NULL);
	i = 0;
	n = t->_cmd;
	while (n && ++i)
		n = n->nxt;
	return (i);
}
