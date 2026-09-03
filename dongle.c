/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:34:15 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/02 19:10:45 by pmarani          ###   ########.fr       */
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

void	acquire_dongle(t_dongle *dongle, int dongle_cooldown)
{
	long	elapsed;

	pthread_mutex_lock(&dongle->mutex);
	elapsed = get_current_time_ms() - dongle->last_release_time;
	while (dongle->state == 1 || elapsed < dongle_cooldown)
	{
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
		elapsed = get_current_time_ms() - dongle->last_release_time;
	}
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
	if (coder->left == coder->right)
	{
		while (is_simulation_over(coder->data) == 0)
			usleep(1000);
		return (1);
	}
	else if (coder->left != coder->right)
	{
		if (coder->left < coder->right)
		{
			acquire_dongle(coder->left, coder->data->params.dongle_cooldown);
			acquire_dongle(coder->right, coder->data->params.dongle_cooldown);
		}
		else
		{
			acquire_dongle(coder->right, coder->data->params.dongle_cooldown);
			acquire_dongle(coder->left, coder->data->params.dongle_cooldown);
		}
	}
	return (0);
}
