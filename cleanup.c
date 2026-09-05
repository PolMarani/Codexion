/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 21:28:41 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 23:49:02 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->params.number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		pthread_cond_destroy(&data->dongles[i].cond);
		pthread_mutex_destroy(&data->coders[i].coder_mutex);
		free(data->dongles[i].waiting_queue);
		i++;
	}
	free(data->dongles);
	free(data->coders);
	free(data->coder_threads);
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->is_simulation_over_mutex);
}
