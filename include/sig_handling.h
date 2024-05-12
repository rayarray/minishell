/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handling.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:36:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 23:21:43 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIG_HANDLING_H
# define SIG_HANDLING_H

// sig_handling.c
void	ignore_signal(int sig);
void	default_signal(int sig);
void	disable_echoctl(void);
void	enable_echoctl(void);

#endif