/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 17:51:37 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 17:06:40 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "minishell.h"
#include "sig_handling.h"
#include "libft.h"
#include "parser.h"
#include "interpreter.h"
#include "environ.h"
#include "vector.h"
#include "input_reader.h"

static void	init_signals(void)
{
	disable_echoctl();
	ignore_signal(SIGINT);
	ignore_signal(SIGQUIT);
}

static int	handle_input(t_mini_env *env, char *input)
{
	t_cmdtab	*ct;
	int			return_value;

	return_value = 0;
	if (check_for_open_quotes_or_pipeline(input))
		return_value = 2;
	else if (*input != '\0' && *input != 3)
	{
		ct = tabler(lexer(input));
		ct = parser(env->env_vec, ct);
		if (ct->error)
		{
			ft_fprintf(STDERR_FILENO,
				"syntax error near unexpected token: `%s'\n", ct->error);
			return_value = 258;
		}
		else
			return_value = evaluate(&ct, env);
		destroy_cmdtab(ct);
	}
	else if (*input == 3)
		return_value = 1;
	return (return_value);
}

static int	main_loop(t_mini_env *env)
{
	int			return_value;
	char		*input;

	return_value = 1;
	input = malloc(sizeof(char) * MINI_ARG_MAX);
	if (input == NULL)
		return (print_strerror(errno, "Input buffer"));
	while (input != NULL && get_input(&input, MINI_ARG_MAX) == 0)
	{
		return_value = handle_input(env, input);
		env->env_vec->extra = return_value;
	}
	ft_fprintf(STDERR_FILENO, "exit\n");
	free(input);
	return (return_value);
}

int	main(void)
{
	int			return_value;
	t_mini_env	env;
	pid_t		pid;

	init_signals();
	return_value = 0;
	pid = fork();
	if (pid == 0)
	{
		ignore_signal(SIGINT);
		ignore_signal(SIGQUIT);
		env.env_vec = env_init();
		if (env.env_vec != NULL)
			return_value = main_loop(&env);
		exit(return_value);
	}
	if (pid < 0)
	{
		enable_echoctl();
		return (print_strerror(errno, "main\n"));
	}
	waitpid(pid, &return_value, 0);
	enable_echoctl();
	return (WEXITSTATUS(return_value));
}
