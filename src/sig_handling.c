/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:36:50 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/18 14:22:57 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <termios.h>
#include "minishell.h"
#include "libft.h"

// Sets given signal to be ignored
void	ignore_signal(int sig)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(sig, &sa, NULL);
}

// Sets given signal to be handled in the default manner
void	default_signal(int sig)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(sig, &sa, NULL);
}

// sets signal handler for SIGNUM, ignores SIGQUIT
// and turns off ECHOCTL for STDIN_FILENO
void	disable_echoctl(void)
{
	struct termios		term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag = term.c_lflag & ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// sets signal handler for SIGNUM, ignores SIGQUIT
// and turns off ECHOCTL for STDIN_FILENO
void	enable_echoctl(void)
{
	struct termios		term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag = term.c_lflag | ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
