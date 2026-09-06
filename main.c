/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 23:48:29 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 13:29:16 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_all(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;

	if (init_simulation(argc, argv, &data) != 0)
		return (1);
	data.init_start_time = get_current_time_ms();
	if (start_threads(&data) != 0)
	{
		fprintf(stderr, "Error creating threads\n");
		cleanup(&data);
		return (1);
	}
	if (start_monitor(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	join_all(&data);
	cleanup(&data);
	return (0);
}

void	join_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->params.number_of_coders)
	{
		pthread_join(data->coder_threads[i], NULL);
		i++;
	}
	pthread_join(data->monitor_threads, NULL);
}