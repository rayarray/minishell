/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_entry_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 19:12:06 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:30:37 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "parser.h"
#include "file_operations.h"

// frees an array of t_file_entries and nulls the pointer. Closes any file
// with fd >= 0 if it exists.
void	free_fearray_and_unlink_temps(t_file_entry ***entries)
{
	t_file_entry	**fe;

	if (!entries)
		return ;
	fe = *entries;
	while (fe && *fe)
	{
		if ((*fe)->fd >= 0)
			close_file_entry(*fe);
		if ((*fe)->mode == IN_A)
			if (access((*fe)->filename, F_OK))
				unlink((*fe)->filename);
		free((*fe)->filename);
		free(*(fe++));
	}
	free(*entries);
	*entries = NULL;
}
