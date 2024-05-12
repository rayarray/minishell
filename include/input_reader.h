/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_reader.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 17:30:43 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/14 23:20:06 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_READER_H
# define INPUT_READER_H

// input_reader.c
int	get_input(char **input, int input_size);

// input_reader_utilities.c
int	is_empty(char *s);
int	check_for_open_quotes_or_pipeline(char *s);

#endif