/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:01:53 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/17 14:09:18 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H
# define ENV_VAR_1_CHARS \
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
# define ENV_VAR_N_CHARS \
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_1234567890"
# define LAST_NODE 1

typedef struct s_heredoc_node
{
	const char	*str;
	int			end;
	int			typ;
}	t_heredoc_node;

// heredoc_utils.c
int	is_var_name_char(char c, int index);
int	count_vars(const char *str);
int	get_var_name_length(const char *s);
int	get_total_length(t_heredoc_node *list);
int	write_string_and_newline(int fd, char *str);

#endif