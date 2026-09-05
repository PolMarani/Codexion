/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 19:02:25 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 00:29:15 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_number(char *str);
int	is_valid_scheduler(char *str);

int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (0);
	i = 1;
	while (i < 8)
	{
		if (is_valid_number(argv[i]) == 0)
			return (0);
		i++;
	}
	if (atoi(argv[1]) <= 0)
	{
		fprintf(stderr,
			"Error: Number of coders must be greater than 0\n");
		return (0);
	}
	if (is_valid_scheduler(argv[i]) == 0)
		return (0);
	return (1);
}

int	is_valid_number(char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	is_valid_scheduler(char *str)
{
	if ((strcmp(str, "fifo") == 0) || (strcmp(str, "edf") == 0))
		return (1);
	return (0);
}

void	parse_params(char **argv, t_params *params)
{
	params->number_of_coders = atoi(argv[1]);
	params->time_to_burnout = atoi(argv[2]);
	params->time_to_compile = atoi(argv[3]);
	params->time_to_debug = atoi(argv[4]);
	params->time_to_refactor = atoi(argv[5]);
	params->number_of_compiles_required = atoi(argv[6]);
	params->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		params->scheduler = 0;
	else
		params->scheduler = 1;
}
