/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:34:15 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 00:34:07 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->state = 0;
	dongle->last_release_time = get_current_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	acquire_dongle(t_dongle *dongle, int coder_id,
		t_data *data, long deadline)
{
	long	elapsed;

	pthread_mutex_lock(&dongle->mutex);
	enqueue_coder(dongle, data, coder_id, deadline);
	elapsed = get_current_time_ms() - dongle->last_release_time;
	while (is_simulation_over(data) == 0)
	{
		elapsed = get_current_time_ms()	- dongle->last_release_time;
		if (dongle->state == 0
				&& dongle->waiting_queue[0].coder_id == coder_id)
		{
			if (elapsed >= data->params.dongle_cooldown)
				break ;
			pthread_mutex_unlock(&dongle->mutex);
			ft_usleep(data->params.dongle_cooldown - elapsed);
			pthread_mutex_lock(&dongle->mutex);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	dequeue_coder(dongle, data);
	if (is_simulation_over(data) == 0)
		dongle->state = 1;
	pthread_mutex_unlock(&dongle->mutex);
}

int	is_simulation_over(t_data *data)
{
	int	value;

	pthread_mutex_lock(&data->is_simulation_over_mutex);
	value = data->is_simulation_over;
	pthread_mutex_unlock(&data->is_simulation_over_mutex);
	return (value);
}

void	set_simulation_over(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->is_simulation_over_mutex);
	data->is_simulation_over = 1;
	pthread_mutex_unlock(&data->is_simulation_over_mutex);
	i = 0;
	while (i < data->params.number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

int	acquire_both_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
	{
		while (is_simulation_over(coder->data) == 0)
			ft_usleep(1);
		return (1);
	}
	acquire_ordered(coder);
	return (0);
}
