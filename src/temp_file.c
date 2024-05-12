/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 19:35:48 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:47:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "file_operations.h"
#include "parser.h"
#include "environ.h"
#include "minishell.h"

// Creates a random string containing only lowercase a-z letters. Returns 0
// on success, and some other value on failure.
static int	get_random_filename(char *bfr, size_t len)
{
	int		randfd;
	size_t	ret_val;

	if (len == 0 || !bfr)
		return (EINVAL);
	randfd = open(RANDOM_SOURCE, O_RDONLY);
	if (randfd < 0)
		return (print_strerror(errno, RANDOM_SOURCE));
	ret_val = read(randfd, bfr, len - 1);
	if (close(randfd))
		return (print_strerror(errno, RANDOM_SOURCE));
	if (ret_val != len - 1)
	{
		ft_fprintf(STDERR_FILENO, "RANDOM_SOURCE read error\n");
		return (-1);
	}
	bfr[(int)--len] = '\0';
	while ((int)--len >= 0)
		bfr[(int)len] = (unsigned char)bfr[(int)len] % 26 + 97;
	return (0);
}

// Attempts to place temp file in $TMPDIR, or if the key is not set, in /tmp. 
// If these cannot be accessed, will try to place the file in current working
// directory. If the random filename is already used, will try to generate a new
// filename 42 times after giving up.
static char	*get_temp_filename(t_mini_env *env)
{
	char	path[PATH_MAX];
	char	filename[TEMP_FILE_LENGTH];
	char	*full_filename;
	int		i;

	ft_strlcpy(path, "/tmp", PATH_MAX - TEMP_FILE_LENGTH - 1);
	if (env_find(env->env_vec, TMPDIR))
		ft_strlcpy(path, env_search(env->env_vec, TMPDIR, 0, -1),
			PATH_MAX - TEMP_FILE_LENGTH - 1);
	if (!is_directory(path) || access(path, W_OK | X_OK) != 0)
		if (getcwd(path, PATH_MAX - TEMP_FILE_LENGTH - 1) == NULL)
			return (NULL);
	i = 0;
	while (i++ < 42)
	{
		if (get_random_filename(filename, TEMP_FILE_LENGTH) != 0)
			return (NULL);
		full_filename = ft_strjoin2(path, filename, '/');
		if (access(full_filename, F_OK) != 0 && errno == ENOENT)
			return (full_filename);
		free(full_filename);
	}
	return (NULL);
}

// Attempts to open a random filename and returns a t_file_entry of the opened
// file. If an error is encountered, NULL is returned.
t_file_entry	*open_random_file(t_mini_env *env)
{
	t_file_entry	*entry;
	char			*full_filename;

	full_filename = get_temp_filename(env);
	entry = new_file_entry(-1, IN_A, full_filename);
	free(full_filename);
	if (entry != NULL)
		entry->fd = open(entry->filename, O_WRONLY | O_CREAT | O_EXCL,
				S_IRUSR | S_IWUSR);
	if (entry == NULL || entry->fd < 0)
	{
		print_strerror(errno, "Temp file creation failed\n");
		free_file_entry(&entry);
	}
	return (entry);
}
