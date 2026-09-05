/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:34:15 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 22:26:45 by pmarani          ###   ########.fr       */
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
	long		elapsed;
	int			was_queued;

	was_queued = 0;
	pthread_mutex_lock(&dongle->mutex);
	elapsed = get_current_time_ms() - dongle->last_release_time;
	if (dongle->state == 1 || elapsed < data->params.dongle_cooldown)
	{
		enqueue_coder(dongle, data, coder_id, deadline);
		was_queued = 1;
	}
	while (dongle->state == 1 || elapsed < data->params.dongle_cooldown
		|| dongle->waiting_queue[0].coder_id != coder_id)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		elapsed = get_current_time_ms() - dongle->last_release_time;
	}
	if (was_queued == 1)
		remove_from_queue(dongle);
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
	pthread_mutex_lock(&data->is_simulation_over_mutex);
	data->is_simulation_over = 1;
	pthread_mutex_unlock(&data->is_simulation_over_mutex);
}

int	acquire_both_dongles(t_coder *coder)
{
	printf("coder trying acquire_both\n");
	if (coder->left == coder->right)
	{
		while (is_simulation_over(coder->data) == 0)
			usleep(1000);
		return (1);
	}
	acquire_ordered(coder);
	return (0);
}
