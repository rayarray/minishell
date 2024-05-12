/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_operations.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 23:02:35 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 23:11:38 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_OPERATIONS_H
# define FILE_OPERATIONS_H
# include "minishell.h"
# define RANDOM_SOURCE "/dev/urandom"
# define TMPDIR "TMPDIR"
# define TEMP_FILE_LENGTH 9

// Data structure for files.
typedef struct s_file_entry
{
	int		fd;
	int		mode;
	char	*filename;
}	t_file_entry;

// file_operations.c
char			**get_env_path_value(const t_mini_env *env);
char			*find_cmd(const char *exe, const t_mini_env *env);
int				check_file_access(const char *filename);
int				is_directory(const char *file);

// file_entry.c
t_file_entry	*new_file_entry(int fd, int mode, const char *filename);
void			free_file_entry(t_file_entry **fe);
int				open_file_entry(t_file_entry *fe);
int				close_file_entry(t_file_entry *fe);

// file_entry_utils.c
void			free_fearray_and_unlink_temps(t_file_entry ***entries);

// temp_file.c
t_file_entry	*open_random_file(t_mini_env *env);

#endif
