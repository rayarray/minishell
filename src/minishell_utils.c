/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 20:15:20 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 16:40:27 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "libft.h"

// Prints the error message associated with errnum to stderr and returns errnum
int	print_strerror(int errnum, const char *s)
{
	char	*output;

	output = strerror(errnum);
	ft_fprintf(STDERR_FILENO, "%s: %s\n", s, output);
	return (errnum);
}

// Like print_strerror but prints only the string and returns errnum.
int	print_error(int errnum, const char *s, const char *suffix)
{
	ft_fprintf(STDERR_FILENO, "%s", s);
	if (suffix)
		ft_fprintf(STDERR_FILENO, ": %s", suffix);
	ft_fprintf(STDERR_FILENO, "\n", s);
	return (errnum);
}

// Frees an allocated string array. Returns *array which should be NULL
char	**free_strarray(char ***array)
{
	char	**i;

	if (!array || !*array)
		return (NULL);
	i = *array;
	while (i && *i != NULL)
		free(*(i++));
	free(*array);
	*array = NULL;
	return (*array);
}

// Allocates a new string array and copies the strings of src into it
char	**copy_strarray(const char **src)
{
	char	**dest;
	int		i;
	int		j;

	if (src == NULL)
		return (NULL);
	i = -1;
	while (src[++i] != NULL)
		;
	dest = malloc(sizeof(char *) * (i + 1));
	if (!dest)
	{
		perror(NULL);
		return (NULL);
	}
	j = -1;
	while (++j < i)
	{
		dest[j] = ft_strdup(src[j]);
		if (dest[j] == NULL)
			return (free_strarray(&dest));
	}
	dest[j] = NULL;
	return (dest);
}

// Does an exact comparison of two strings. Returns 1 if they're identical and
// otherwise 0.
int	ft_strcmp(const char *s1, const char *s2)
{
	while (s1 && s2 && *s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
		if (*s1 == 0 && *s2 == 0)
			return (1);
	}
	return (0);
}
