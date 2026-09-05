/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 11:20:52 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 12:57:03 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	enqueue_coder(t_dongle *dongle, t_data *data,
	int coder_id, long deadline)
{
	t_waiter	w;

	if (data->params.scheduler == 0)
	{
		dongle->waiting_queue[dongle->waiting_cont].coder_id = coder_id;
		dongle->waiting_queue[dongle->waiting_cont].deadline = deadline;
		dongle->waiting_cont++;
	}
	else
	{
		w.coder_id = coder_id;
		w.deadline = deadline;
		heap_push(dongle, w);
	}
}

void	dequeue_coder(t_dongle *dongle, t_data *data)
{
	if (data->params.scheduler == 0)
		remove_from_queue(dongle);
	else
		heap_pop(dongle);
}
