/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:36:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/06 19:57:08 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// checks for unprintable characters and whitespace characters
int	ft_isspace(char c)
{
	return ((c >= 0 && c < 33) || c == 127);
}
