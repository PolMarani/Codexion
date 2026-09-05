/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 11:20:52 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 00:21:41 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_higher_priority(t_waiter a, t_waiter b)
{
	if (a.deadline < b.deadline)
		return (1);
	if (a.deadline == b.deadline && a.coder_id < b.coder_id)
		return (1);
	return (0);
}

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
	if (dongle->waiting_cont <= 0)
		return ;
	if (data->params.scheduler == 0)
		remove_from_queue(dongle);
	else
		heap_pop(dongle);
}
