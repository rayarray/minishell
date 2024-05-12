/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 23:02:29 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 16:49:47 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "file_operations.h"
#include "libft.h"
#include "minishell.h"
#include "environ.h"

static char	*get_full_path(char const *path, char const *file)
{
	char	*result;
	int		resultlength;

	if (path == NULL)
		return (ft_strdup(file));
	if (path[ft_strlen(path) - 1] == '/')
		result = ft_strjoin(path, file);
	else
	{
		resultlength = ft_strlen(path) + ft_strlen(file) + 1;
		result = malloc(sizeof(char) * (resultlength + 1));
		if (result == NULL)
			return (NULL);
		ft_strlcpy(result, path, resultlength + 1);
		result[ft_strlen(path)] = '/';
		result[ft_strlen(path) + 1] = '\0';
		ft_strlcat(result, file, resultlength + 1);
	}
	return (result);
}

// Returns the directories of $PATH in string array
char	**get_env_path_value(const t_mini_env *env)
{
	char	**paths;
	char	*path;

	path = env_search(env->env_vec, "PATH", 0, -1);
	if (path == NULL)
		return (NULL);
	paths = ft_split(path, ':');
	return (paths);
}

// Searches for the given exe along $PATH or, if given an exact path,
// returns that.
char	*find_cmd(const char *exe, const t_mini_env *env)
{
	char	**env_path;
	char	**path_iterator;
	char	*full_location;

	if (exe == NULL)
		return (NULL);
	if (ft_strchr(exe, '/'))
		return (ft_strdup(exe));
	env_path = get_env_path_value(env);
	path_iterator = env_path;
	while (path_iterator && *path_iterator)
	{
		full_location = get_full_path(*(path_iterator++), exe);
		if (access(full_location, F_OK) == 0)
		{
			if (is_directory(full_location) == 0)
			{
				free_strarray(&env_path);
				return (full_location);
			}
		}
		free(full_location);
	}
	free_strarray(&env_path);
	return (ft_strdup(exe));
}

// Checks if the given file exists and if it can be executed. Returns 0 if 
// it can be executed.
int	check_file_access(const char *filename)
{
	if (!ft_strchr(filename, '/'))
	{
		ft_fprintf(STDERR_FILENO, "command not found: %s\n", filename);
		return (127);
	}
	if (access(filename, F_OK) != 0)
	{
		if (ft_strchr(filename, '/'))
			print_strerror(errno, filename);
		else if (filename == NULL)
			ft_fprintf(STDERR_FILENO, " : command not found\n");
		else
			ft_fprintf(STDERR_FILENO, "%s: command not found\n", filename);
		return (127);
	}
	if (is_directory(filename))
		return (print_error(126, filename, "is a directory"));
	if (access(filename, X_OK) != 0)
		return (print_error(126, filename, "Permission denied"));
	return (0);
}

// Returns 1 if the given filename refers to a directory and 0 if not
int	is_directory(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY);
	if (fd < 0 && errno == EISDIR)
		return (1);
	if (fd >= 0)
		close(fd);
	return (0);
}
