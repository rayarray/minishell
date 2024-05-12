/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 09:56:45 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/11 18:43:52 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stdlib.h>

// size: nbr of units in vector (first = 0)
typedef struct s_strvec {
	char	**vec;
	int		size;
	int		max;
	int		extra;
	char	extra_str[11];
}	t_strvec;

// vector_s.c

t_strvec	*init_strvec(int size, char **s, int copy);
int			rsz_strvec(t_strvec *v, int grow);
int			add_strvec(t_strvec *v, char *s, int copy);
int			rm_strvec(t_strvec *v, char *s, int order, int fr);
void		destroy_strvec(t_strvec *v, int fr);

#endif	/* VECTOR_H */