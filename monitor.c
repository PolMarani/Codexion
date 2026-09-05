/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 12:12:03 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 00:38:22 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	while (is_simulation_over(data) == 0)
	{
		if (check_coders(data) != 0)
			return (NULL);
		ft_usleep(1);
	}
	return (NULL);
}

int	check_coders(t_data *data)
{
	int		i;
	int		all_done;
	int		total;

	i = 0;
	all_done = 1;
	while (i < data->params.number_of_coders)
	{
		if (is_burned_out(data, i) == 1)
			return (1);
		pthread_mutex_lock(&data->coders[i].coder_mutex);
		total = data->coders[i].total_compiled;
		pthread_mutex_unlock(&data->coders[i].coder_mutex);
		if (total < data->params.number_of_compiles_required)
			all_done = 0;
		i++;
	}
	if (all_done == 1)
	{
		set_simulation_over(data);
		return (2);
	}
	return (0);
}

int	is_burned_out(t_data *data, int i)
{
	long	last_compile;

	pthread_mutex_lock(&data->coders[i].coder_mutex);
	last_compile = data->coders[i].last_start_compile;
	pthread_mutex_unlock(&data->coders[i].coder_mutex);
	if (get_current_time_ms() - last_compile > data->params.time_to_burnout)
	{
		print_status(&data->coders[i], "burned out");
		set_simulation_over(data);
		return (1);
	}
	return (0);
}
