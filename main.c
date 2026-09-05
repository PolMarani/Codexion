/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 23:48:29 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 21:34:46 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (init_simulation(argc, argv, &data) != 0)
		return (1);
	data.init_start_time = get_current_time_ms();
	i = 0;
	if (start_threads(&data) != 0)
	{
		fprintf(stderr, "Error creating threads\n");
		return (1);
	}
	pthread_create(&data.monitor_threads, NULL, monitor_routine, &data);
	i = 0;
	while (i < data.params.number_of_coders)
	{
		pthread_join(data.coder_threads[i], NULL);
		i++;
	}
	pthread_join(data.monitor_threads, NULL);
	cleanup(&data);
	return (0);
}
