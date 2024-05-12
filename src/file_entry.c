/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_entry.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/24 16:27:22 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_operations.h"
#include "parser.h"
#include "libft.h"
#include "interpreter.h"

static int	validate_arguments(int mode, const char *filename)
{
	if (filename == NULL)
		ft_fprintf(STDERR_FILENO, "Cannot open file %s\n", filename);
	else if (ft_strlen(filename) > PATH_MAX)
		print_strerror(ENAMETOOLONG, filename);
	else if (mode != IN && mode != IN_A && mode != OUT && mode != OUT_A)
		ft_fprintf(STDERR_FILENO, "%s: Illegal file mode %d\n", filename, mode);
	else
		return (1);
	return (0);
}

// Creates a new file entry.
t_file_entry	*new_file_entry(int fd, int mode, const char *filename)
{
	t_file_entry	*new_entry;

	new_entry = NULL;
	if (validate_arguments(mode, filename))
	{
		new_entry = malloc(sizeof(t_file_entry));
		if (new_entry == NULL)
			print_strerror(errno, filename);
		else
		{
			new_entry->fd = fd;
			new_entry->mode = mode;
			new_entry->filename = ft_strdup(filename);
			if (new_entry->filename == NULL)
			{
				print_strerror(errno, filename);
				free(new_entry);
				new_entry = NULL;
			}
		}
	}
	return (new_entry);
}

void	free_file_entry(t_file_entry **fe)
{
	if (fe && *fe)
	{
		free((*fe)->filename);
		free(*fe);
		*fe = NULL;
	}
}

int	open_file_entry(t_file_entry *fe)
{
	int	fd;
	int	access;

	access = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	fd = -1;
	if (fe->fd >= 1)
	{
		ft_fprintf(STDERR_FILENO, "%s: Already open\n", fe->filename);
		return (-1);
	}
	if (fe->mode == IN || fe->mode == IN_A)
		fd = open(fe->filename, O_RDONLY);
	else if (fe->mode == OUT)
		fd = open(fe->filename, O_CREAT | O_TRUNC | O_WRONLY, access);
	else if (fe->mode == OUT_A)
		fd = open(fe->filename, O_CREAT | O_APPEND | O_WRONLY, access);
	if (fd < 0)
		return (print_strerror(errno, fe->filename));
	fe->fd = fd;
	return (0);
}

int	close_file_entry(t_file_entry *fe)
{
	if (close(fe->fd) == 0)
	{
		fe->fd = -1;
		return (0);
	}
	return (print_strerror(errno, fe->filename));
}
