/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 22:16:36 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 13:40:25 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_coders(t_dongle *dongles, t_coder *coders,
					t_data *data, int number_of_coders)
{
	int			i;

	i = 0;
	while (i < number_of_coders)
	{
		coders[i].coder_number = i + 1;
		coders[i].total_compiled = 0;
		coders[i].last_start_compile = get_current_time_ms();
		pthread_mutex_init(&coders[i].coder_mutex, NULL);
		coders[i].params = &data->params;
		coders[i].right = &dongles[i];
		coders[i].left = &dongles[
			(i - 1 + number_of_coders) % number_of_coders];
		coders[i].data = data;
		i++;
	}
}

int	create_dongles(t_dongle *dongles, int number_of_coders,
						int dongle_cooldown)
{
	int	i;

	i = -1;
	while (++i < number_of_coders)
	{
		dongles[i].state = 0;
		dongles[i].last_release_time = -dongle_cooldown;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		dongles[i].waiting_queue = malloc(sizeof(t_waiter) * number_of_coders);
		if (!dongles[i].waiting_queue)
		{
			pthread_mutex_destroy(&dongles[i].mutex);
			pthread_cond_destroy(&dongles[i].cond);
			while (i--)
			{
				pthread_mutex_destroy(&dongles[i].mutex);
				pthread_cond_destroy(&dongles[i].cond);
				free(dongles[i].waiting_queue);
			}
			return (1);
		}
		dongles[i].waiting_cont = 0;
	}
	return (0);
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
	if (create_dongles(data->dongles, data->params.number_of_coders,
			data->params.dongle_cooldown) == 1)
	{
		free(data->dongles);
		free(data->coders);
		free(data->coder_threads);
		return (1);
	}
	create_coders(data->dongles, data->coders, data,
		data->params.number_of_coders);
	data->is_simulation_over = 0;
	pthread_mutex_init(&data->is_simulation_over_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
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
	data->coder_threads = malloc(sizeof(pthread_t)
			* data->params.number_of_coders);
	if (!data->coder_threads)
	{
		fprintf(stderr, "Error in creating malloc threads of the coders\n");
		free(data->dongles);
		free(data->coders);
		return (1);
	}
	return (0);
}

int	start_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->params.number_of_coders)
	{
		if (pthread_create(&data->coder_threads[i], NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			set_simulation_over(data);
			while (--i >= 0)
				pthread_join(data->coder_threads[i], NULL);
			return (1);
		}
		i++;
	}
	return (0);
}
