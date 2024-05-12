/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 14:40:36 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/21 14:04:07 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "vector.h"
# include <stdlib.h>

# define EXEC	0
# define IN		-1
# define IN_A	-2
# define OUT	-3
# define OUT_A	-4
# define PIPE	-5

// intermediate form: str points starting point in char * given 
// by readline, end indicates how many chars to read
// in final form str is malloced separately and end is not needed 
// (perhaps set to -1 when malloced)
typedef struct s_node {
	const char		*str;
	int				end;
	int				typ;
	struct s_node	*nxt;
}	t_node;

// 2 lists, t_node is the unit, null-terminated
// _cmd = first node is the executable, the rest are arguments
// redir = redirections 
// cmd = cmd and arguments in char **
// pipe points to a piped command, if there is one, NULL if not
// error is NULL if syntax is valid, otherwise error is set to char before error
typedef struct s_cmdtab {
	t_node			*_cmd;
	char			**cmd;
	t_node			*redir;
	struct s_cmdtab	*pipe;
	char			*error;
	char			quote;
}	t_cmdtab;

// struct for refactored expander
typedef struct s_expander2 {
	const char		*s;
	int				i;
	int				j;
	int				e_i;
	char			qt;
	char			*env;
	int				envlen;
	int				expand_len;
	char			*env_str;
	char			*final_str;
	int				bgn;
	int				end;
	int				qtchrs;
	int				wschrs;
}	t_expander2;

// parser.c

t_cmdtab	*parser(t_strvec *env, t_cmdtab *t);
t_node		*lexer(const char *s);
t_cmdtab	*tabler(t_node *n);

// expander.c

t_node		*expand(t_strvec *e, t_node *n, char *qt);
void		expand_env(t_expander2 *x, t_strvec *e, char *dst);
int			expand_envs(t_expander2 *x, t_strvec *e, t_node *n, char *dst);

// parser_utils1.c

t_node		*init_node(const char *str, int end, int typ);
t_node		*add_last(t_node *f, t_node *n);
int			is_ctrl(const char *s);
char		*type2errorstr(int err_type);

// parser_utils2.c

int			ft_findchrstr(char c, char *s);
int			isenvchar(char c, int i);
void		destroy_cmdtab(t_cmdtab *ct);
int			destroy_emptynodes(t_cmdtab *t);

#endif	/* PARSER_H */
