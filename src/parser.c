/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 14:40:49 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/16 23:27:59 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_node	*parse_arg(const char *s, char c, int i, int j)
{
	while (s && *s && ft_isspace(*s))
		s++;
	if (s && *s && is_ctrl(s) != 0)
		j = 0 - is_ctrl(s++);
	if (j == OUT_A || j == IN_A)
		s++;
	while (s && *s && ft_isspace(*s))
		s++;
	while (j != PIPE && s && s[i]
		&& (c != ' ' || (s[i] != 32 && !is_ctrl(s + i))))
	{
		while (s[i] && (c != ' ' || (s[i] != ' ' && is_ctrl(s + i) == 0)))
		{
			if (s[i] == '"' || s[i] == '\'')
			{
				if (c == s[i])
					c = ' ';
				else if (c == ' ')
					c = s[i];
			}
			i++;
		}
	}
	return (init_node(s, i, j));
}

t_node	*lexer(const char *s)
{
	t_node	*f;
	t_node	*n;

	n = parse_arg(s, ' ', 0, 0);
	if (n == NULL)
		return (NULL);
	f = n;
	s = n->str + n->end;
	while (s && *s && ft_isspace(*s))
		s++;
	while (s && *s)
	{
		n->nxt = parse_arg(s, ' ', 0, 0);
		if (n->nxt == NULL)
			return (NULL);
		n = n->nxt;
		s = n->str + n->end;
		while (*s && ft_isspace(*s))
			s++;
	}
	return (f);
}

t_cmdtab	*tabler(t_node *n)
{
	t_cmdtab	*t;
	t_node		*i;

	t = (t_cmdtab *)malloc(sizeof(t_cmdtab));
	if (t == NULL)
		return (NULL);
	ft_bzero(t, sizeof(t_cmdtab));
	while (n && n->typ != -5)
	{
		i = n->nxt;
		n->nxt = NULL;
		if (n->typ == 0)
			t->_cmd = add_last(t->_cmd, n);
		else if (n->typ <= -1 && n->typ >= -4)
			t->redir = add_last(t->redir, n);
		n = i;
	}
	if (n && n->typ == -5)
	{
		i = n->nxt;
		free(n);
		t->pipe = tabler(i);
	}
	return (t);
}

char	*errorchecker(t_cmdtab *t, t_node *n)
{
	if (t->pipe && (!*t->cmd || **t->cmd == 0) && !t->redir
		&& t->_cmd && t->_cmd->end != -1)
		return (ft_strdup("|"));
	n = t->redir;
	while (n)
	{
		if (n && (!n->str || *n->str == 0) && n->end != -1)
		{
			if (n->nxt && n->nxt->typ <= -1 && n->nxt->typ >= -4)
				return (type2errorstr(n->nxt->typ));
			else if (t->pipe)
				return (ft_strdup("|"));
			return (ft_strdup("newline"));
		}
		n = n->nxt;
	}
	if (t->pipe && *(t->pipe->cmd) == 0 && !t->pipe->redir)
		return (ft_strdup("|"));
	else if (t->pipe)
		return (errorchecker(t->pipe, NULL));
	return (NULL);
}

t_cmdtab	*parser(t_strvec *env, t_cmdtab *t)
{
	t_node	*n;
	int		i;

	n = t->_cmd;
	while (n)
		n = expand(env, n, &t->quote);
	n = t->redir;
	while (n)
		n = expand(env, n, &t->quote);
	i = destroy_emptynodes(t);
	n = t->_cmd;
	t->cmd = (char **)malloc(sizeof(char *) * (i + 1));
	if (t->cmd == NULL)
		return (NULL);
	(t->cmd)[i] = 0;
	i = 0;
	while (n)
	{
		(t->cmd)[i++] = (char *)n->str;
		n = n->nxt;
	}
	if (t->pipe)
		parser(env, t->pipe);
	t->error = errorchecker(t, NULL);
	return (t);
}
