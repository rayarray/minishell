/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rleskine <rleskine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 16:28:56 by rleskine          #+#    #+#             */
/*   Updated: 2023/09/20 19:54:04 by rleskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRON_H
# define ENVIRON_H

# include "vector.h"

// environ.c

t_strvec	*env_init(void);
int			env_find(t_strvec *env, const char *s);
char		*env_getval(t_strvec *env, const char *s);
char		*env_search(t_strvec *env, const char *s, int full, int len);
int			env_add(t_strvec *env, char *s);

// environ2.c

char		*exit_value_str(t_strvec *e);
int			env_add_kv(t_strvec *env, const char *key, const char *val);
int			set_shlvl(t_strvec *env);
int			env_exist_val(t_strvec *env, const char *s);

#endif	/* ENVIRON_H */
