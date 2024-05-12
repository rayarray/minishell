/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstgetmatch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 19:30:57 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/24 20:13:11 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Removes the first matching item from the list and returns it.
t_list	*ft_lstgetmatch(t_list **list, int (*f)(void *, void *), void *term)
{
	t_list	*iterator;
	t_list	*match;

	if (*list == NULL)
		return (NULL);
	iterator = *list;
	if (f(iterator->content, term))
	{
		*list = (*list)->next;
		iterator->next = NULL;
		return (iterator);
	}
	while (iterator->next != NULL)
	{
		if (f(iterator->next->content, term))
		{
			match = iterator->next;
			iterator->next = iterator->next->next;
			match->next = NULL;
			return (match);
		}
		else
			iterator = iterator->next;
	}
	return (NULL);
}
