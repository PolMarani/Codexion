/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:16:36 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/02 17:37:09 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_coders(t_dongle *dongles, t_coder *coders,
					t_params *params, int number_of_coders)
{
	int			i;

	i = 0;
	while (i < number_of_coders)
	{
		coders[i].coder_number = i + 1;
		coders[i].total_compiled = 0;
		coders[i].last_start_compile = 0;
		coders[i].params = params;
		coders[i].right = &dongles[i];
		coders[i].left = &dongles[
			(i - 1 + number_of_coders) % number_of_coders];
		i++;
	}
}

void	create_dongles(t_dongle *dongles, int number_of_coders,
						int dongle_cooldown)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		dongles[i].state = 0;
		dongles[i].last_release_time = -dongle_cooldown;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		i++;
	}
}

int	init_simulation(int argc, char **argv, t_data *data)
{
	if (validate_args(argc, argv) == 0)
	{
		fprintf(stderr, "Invalid args\n");
		return (1);
	}
	parse_params(argv, &data->params);
	if (allocate_arrays(data) == 1)
		return (1);
	create_dongles(data->dongles, data->params.number_of_coders,
		data->params.dongle_cooldown);
	create_coders(data->dongles, data->coders, &data->params,
		data->params.number_of_coders);
	data->is_simulation_over = 0;
	pthread_mutex_init(&data->is_simulation_over_mutex, NULL);
	return (0);
}

int	allocate_arrays(t_data *data)
{
	data->dongles = malloc(sizeof(t_dongle) * data->params.number_of_coders);
	if (!data->dongles)
	{
		fprintf(stderr, "Error in creating malloc dongles\n");
		return (1);
	}
	data->coders = malloc(sizeof(t_coder) * data->params.number_of_coders);
	if (!data->coders)
	{
		fprintf(stderr, "Error in creating malloc coders\n");
		free(data->dongles);
		return (1);
	}
	return (0);
}
