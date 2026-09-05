/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 22:13:32 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 22:25:35 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *str)
{
	long	timestamp;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (is_simulation_over(coder->data) == 0)
	{
		timestamp = get_current_time_ms() - coder->data->init_start_time;
		printf("%ld %d %s\n", timestamp, coder->coder_number, str);
	}
	pthread_mutex_unlock(&coder->data->log_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (is_simulation_over(coder->data) == 0)
	{
		if (acquire_both_dongles(coder) == 1)
			return (NULL);
		print_status(coder, "has taken a dongle");
		print_status(coder, "has taken a dongle");
		print_status(coder, "is compiling");
		ft_usleep(coder->data->params.time_to_compile);
		coder->last_start_compile = get_current_time_ms();
		release_dongle(coder->left);
		release_dongle(coder->right);
		coder->total_compiled++;
		print_status(coder, "is debugging");
		ft_usleep(coder->data->params.time_to_debug);
		print_status(coder, "is refactoring");
		ft_usleep(coder->data->params.time_to_refactor);
	}
	return (NULL);
}
