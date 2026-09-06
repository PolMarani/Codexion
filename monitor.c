/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 12:12:03 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 13:20:16 by pmarani          ###   ########.fr       */
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

int	check_all_done(t_data *data)
{
	int		i;
	int		total;

	i = 0;
	while (i < data->params.number_of_coders)
	{
		pthread_mutex_lock(&data->coders[i].coder_mutex);
		total = data->coders[i].total_compiled;
		pthread_mutex_unlock(&data->coders[i].coder_mutex);
		if (total < data->params.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

int	check_coders(t_data *data)
{
	int		i;

	if (check_all_done(data) == 1)
	{
		set_simulation_over(data);
		return (2);
	}
	i = 0;
	while (i < data->params.number_of_coders)
	{
		if (is_burned_out(data, i) == 1)
			return (1);
		i++;
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

int	start_monitor(t_data *data)
{
	int	i;

	if (pthread_create(&data->monitor_threads, NULL,
			monitor_routine, data) == 0)
		return (0);
	set_simulation_over(data);
	i = 0;
	while (i < data->params.number_of_coders)
	{
		pthread_join(data->coder_threads[i], NULL);
		i++;
	}
	return (1);
}