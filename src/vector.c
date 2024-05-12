/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 10:02:08 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/18 15:33:19 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vector.h"
#include "libft.h"

// size = vector size, will be overwritten if given **s has bigger size
// **s = char array to copy to vector
// copy = 0 if copy pointers, 1 if copy content (malloc)
t_strvec	*init_strvec(int size, char **s, int copy)
{
	t_strvec	*vec;
	char		**ss;
	int			i;

	i = 0;
	ss = s;
	while (ss && *ss && ++i)
		ss++;
	if (i > size)
		size = i;
	vec = (t_strvec *)malloc(sizeof(t_strvec));
	if (vec == NULL)
		return (NULL);
	vec->vec = (char **)malloc(sizeof(char *) * (size + 1));
	if (vec->vec == NULL)
	{
		free (vec);
		return (NULL);
	}
	ft_bzero(vec->vec, sizeof(char *) * (size + 1));
	vec->size = 0;
	vec->max = size;
	while (s && *s)
		add_strvec(vec, *(s++), copy);
	return (vec);
}

// resize vector, 1 = add space, 0 = decrease to size
// returns 1 if successful
int	rsz_strvec(t_strvec *v, int grow)
{
	char	**new;
	int		i;

	i = 0;
	if (grow)
		v->max *= 2;
	new = (char **)malloc(sizeof(char *) * (v->max + 1));
	if (new == NULL && grow)
		v->max /= 2;
	if (new == NULL)
		return (0);
	ft_bzero(new, sizeof(char *) * (v->max + 1));
	new[v->max] = 0;
	while ((v->vec)[i])
	{
		new[i] = (v->vec)[i];
		i++;
	}
	free(v->vec);
	v->vec = new;
	return (1);
}

// add a string to given vector, copy 0 copies pointer, copy 1 copies string
// returns 1 if successful
int	add_strvec(t_strvec *v, char *s, int copy)
{
	if (!v || !s || !*s)
		return (0);
	if (copy)
		s = ft_strdup(s);
	if (copy && !s)
		return (0);
	if (v->size + 1 > v->max && !rsz_strvec(v, 1))
		return (0);
	(v->vec)[v->size] = s;
	(v->size)++;
	return (1);
}

// remove a string, order 0 does not preserve order, order 1 preserves order
// fr 0 = no free, fr 1 = free the removed str
// returns 1 if found and removed
int	rm_strvec(t_strvec *v, char *s, int order, int fr)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	while ((v->vec)[i] && (v->vec)[i] != s)
		i++;
	if ((v->vec)[i] == 0 || i >= v->size)
		return (0);
	if (!order)
	{
		(v->vec)[i] = (v->vec)[v->size - 1];
		(v->vec)[v->size - 1] = NULL;
	}
	while (order && i < v->size)
	{
		(v->vec)[i] = (v->vec)[i + 1];
		i++;
	}
	v->size -= 1;
	if (fr)
		free(s);
	return (1);
}

// destroy/free a given strvec, fr 0 doesn't free strings, fr 1 does
void	destroy_strvec(t_strvec *v, int fr)
{
	if (!v)
		return ;
	while (v->vec && fr && v->size >= 0)
		free((v->vec)[(v->size)--]);
	if (v->vec)
		free(v->vec);
	free(v);
}
