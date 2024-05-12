/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 12:50:10 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/21 13:52:42 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "environ.h"
#include "libft.h"

char	*expand_ws_qt2(t_expander2 *x, char *s, char *dst)
{
	while (*(s + x->i) && (!dst || (dst && x->i < x->end)))
	{
		if ((*(s + x->i) == '\'' || *(s + x->i) == '"') && x->qt == 0)
			x->qt = *(s + x->i) + (0 * ++x->qtchrs);
		else if ((*(s + x->i) == '\'' || *(s + x->i) == '"')
			&& x->qt == *(s + x->i))
			x->qt = (0 * ++x->qtchrs);
		else if (x->i > x->bgn && x->qt == 0 && *(s + x->i) && *(s + x->i + 1)
			&& ft_isspace(*(s + x->i)) && ft_isspace(*(s + x->i + 1))
			&& (!dst || (dst && x->i < x->end)))
		{
			if (dst)
				*(dst + x->i - x->bgn - x->qtchrs - x->wschrs) = ' ';
			while (*(s + x->i + 1) && ft_isspace(*(s + x->i + 1))
				&& (!dst || (dst && x->i < x->end)) && ++x->i)
				x->wschrs++;
		}
		else if (dst)
			*(dst + x->i - x->bgn - x->qtchrs - x->wschrs) = *(s + x->i);
		x->i++;
	}
	if (dst)
		*(dst + x->end - x->bgn - x->qtchrs - x->wschrs) = 0;
	return (dst);
}

char	*expand_ws_qt(t_expander2 *x, char *s, char *dst)
{
	if (!s)
		return (NULL);
	x->i = 0;
	x->qt = 0;
	x->qtchrs = 0;
	x->wschrs = 0;
	while (*(s + x->i) && ft_isspace(*(s + x->i)))
		x->i++;
	x->bgn = x->i;
	dst = expand_ws_qt2(x, s, dst);
	if (dst)
		return (dst);
	x->end = x->i;
	if (x->end > x->bgn && *(s + x->end - 1) && ft_isspace(*(s + x->end - 1)))
		x->end--;
	dst = (char *)malloc(sizeof(char)
			* (x->end - x->bgn - x->qtchrs - x->wschrs + 1));
	if (!dst)
		return (NULL);
	*(dst + x->end - x->bgn - x->qtchrs - x->wschrs) = 0;
	return (expand_ws_qt(x, s, dst));
}

void	expand_env(t_expander2 *x, t_strvec *e, char *dst)
{
	x->e_i = 0;
	x->s++;
	x->i--;
	while (x->i > 0 && isenvchar(*(x->s + x->e_i), x->e_i))
	{
		x->i--;
		x->e_i++;
		if (x->e_i == 1 && *x->s == '?')
			break ;
	}
	if (x->e_i == 1 && *x->s == '?')
		x->env = env_getval(e, "?");
	else
		x->env = env_search(e, x->s, 0, x->e_i);
	x->s += x->e_i;
	x->envlen = 0;
	if (x->env)
		x->envlen = ft_strlen(x->env);
	while (dst && x->envlen != 0 && x->envlen--)
		*(dst + x->j + x->envlen) = *(x->env + x->envlen);
	if (dst && x->env)
		x->envlen = ft_strlen(x->env);
	x->j += x->envlen;
}

// refactored expander, if dst == NULL only returns expanded size
// else copies expanded node to dst
int	expand_envs(t_expander2 *x, t_strvec *e, t_node *n, char *dst)
{
	x->s = n->str;
	x->i = n->end;
	x->j = 0;
	x->qt = 0;
	while (x->i > 0)
	{
		if ((*x->s == '\'' || *x->s == '"') && x->qt == 0)
			x->qt = *x->s;
		else if ((*x->s == '\'' || *x->s == '"') && x->qt == *x->s)
			x->qt = 0;
		if (*x->s == '$' && (x->qt == 0 || x->qt == '"') && *(x->s + 1)
			&& isenvchar(*(x->s + 1), 0) && n->typ != IN_A)
			expand_env(x, e, dst);
		else
		{
			if (dst)
				*(dst + x->j) = *x->s;
			x->j++;
			x->s++;
			x->i--;
		}
	}
	return (x->j);
}

// expands node, returns next node in list
t_node	*expand(t_strvec *e, t_node *n, char *qt)
{
	t_expander2	x;

	ft_bzero(&x, sizeof(t_expander2));
	x.expand_len = expand_envs(&x, e, n, NULL);
	if (x.expand_len <= 0)
	{
		n->end = 0;
		n->str = NULL;
		return (n->nxt);
	}
	x.env_str = (char *)malloc(sizeof(char) * (x.expand_len + 1));
	*(x.env_str + x.expand_len) = 0;
	expand_envs(&x, e, n, x.env_str);
	x.final_str = expand_ws_qt(&x, x.env_str, NULL);
	*qt = x.qt;
	if (x.qtchrs > 0)
		n->end = -1;
	if (x.env_str)
		free(x.env_str);
	n->str = x.final_str;
	return (n->nxt);
}
