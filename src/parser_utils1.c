/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 13:08:01 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/14 22:45:19 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_node	*init_node(const char *s, int end, int typ)
{
	t_node	*n;

	n = (t_node *)malloc(sizeof(t_node));
	if (n == NULL)
		return (NULL);
	n->str = s;
	n->end = end;
	n->typ = typ;
	n->nxt = NULL;
	return (n);
}

t_node	*add_last(t_node *f, t_node *n)
{
	t_node	*i;

	if (f == NULL)
		return (n);
	i = f;
	while (i->nxt)
		i = i->nxt;
	i->nxt = n;
	return (f);
}

int	is_ctrl(const char *s)
{
	if (!s || *s == 0)
		return (0);
	if (*s == '|')
		return (-PIPE);
	if (*s == '>')
	{
		if (*(s + 1) == '>')
			return (-OUT_A);
		return (-OUT);
	}
	if (*s == '<')
	{
		if (*(s + 1) == '<')
			return (-IN_A);
		return (-IN);
	}
	return (0);
}

char	*type2errorstr(int err_type)
{
	if (err_type == -1)
		return (ft_strdup("<"));
	else if (err_type == -2)
		return (ft_strdup("<<"));
	else if (err_type == -3)
		return (ft_strdup(">"));
	else if (err_type == -4)
		return (ft_strdup(">>"));
	else if (err_type == -5)
		return (ft_strdup("|"));
	return (NULL);
}
