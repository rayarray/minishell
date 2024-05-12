/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpreter.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 18:14:54 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/19 16:05:10 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERPRETER_H
# define INTERPRETER_H
# include <sys/types.h>
# include "minishell.h"
# include "environ.h"
# include "parser.h"
# include "file_operations.h"

// Header entry for a child process, containing pid, cmd (command + arguments),
// redirection arguments, stdin redirection fd, stdout redirection fd, pipe fds
// and terminated value that indicates if the process has ended
typedef struct s_proc_hdr
{
	pid_t			pid;
	char			**cmd;
	t_file_entry	**redirects;
	int				stdin_redir;
	int				stdout_redir;
	int				pipe[2];
	int				terminated;
}	t_proc_hdr;

// Evaluating
int				evaluate(t_cmdtab **ct, t_mini_env *env);
int				close_pipe(t_proc_hdr *header);

// Process headers
int				init_process_header(t_proc_hdr *ph);
int				get_headers(t_cmdtab *ct, t_proc_hdr **hdrs, t_mini_env *env);
int				free_proc_hdrs(t_proc_hdr **hdrs, int header_count);
int				free_proc_hdr(t_proc_hdr *hdr);

// Heredoc things
t_file_entry	*heredoc_parser(const char *lim, t_mini_env *env, int var_exp);

//Utils
void			close_pipes_backwards(t_proc_hdr *headers, int header_count);
int				open_pipes(t_proc_hdr *headers, int header_count);

// command_executor
int				exec_cmd(t_proc_hdr *hdr, t_mini_env *env);
int				start_processes(t_proc_hdr *hdrs, int hdr_c, t_mini_env *env);

#endif