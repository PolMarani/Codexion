/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 01:41:50 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/05 11:03:02 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	remove_from_queue(t_dongle *dongle)
{
	int	i;

	i = 0;
	while (i < dongle->waiting_cont - 1)
	{
		dongle->waiting_queue[i] = dongle->waiting_queue[i + 1];
		i++;
	}
	dongle->waiting_cont--;
}

void	acquire_ordered(t_coder *coder)
{
	long	deadline;

	deadline = coder->last_start_compile + coder->data->params.time_to_burnout;
	if (coder->left < coder->right)
	{
		acquire_dongle(coder->left, coder->coder_number,
			coder->data, deadline);
		acquire_dongle(coder->right, coder->coder_number,
			coder->data, deadline);
	}
	else
	{
		acquire_dongle(coder->right, coder->coder_number,
			coder->data, deadline);
		acquire_dongle(coder->left, coder->coder_number,
			coder->data, deadline);
	}
}

void	heap_push(t_dongle *dongle, t_waiter waiter)
{
	int			i;
	int			parent;
	t_waiter	tmp;

	dongle->waiting_queue[dongle->waiting_cont] = waiter;
	dongle->waiting_cont++;
	i = dongle->waiting_cont - 1;
	parent = (i - 1) / 2;
	while (i > 0
		&& (dongle->waiting_queue[parent].deadline
			> dongle->waiting_queue[i].deadline))
	{
		tmp = dongle->waiting_queue[i];
		dongle->waiting_queue[i] = dongle->waiting_queue[parent];
		dongle->waiting_queue[parent] = tmp;
		i = parent;
		parent = (i - 1) / 2;
	}
}

t_waiter	heap_pop(t_dongle *dongle)
{
	t_waiter	result;
	int			i;

	result = dongle->waiting_queue[0];
	dongle->waiting_queue[0] = dongle->waiting_queue[dongle->waiting_cont - 1];
	dongle->waiting_cont--;
	i = 0;
	sift_down(dongle, i);
	return (result);
}

void	sift_down(t_dongle *dongle, int i)
{
	int			smallest;
	t_waiter	tmp;

	while ((2 * i + 1) < dongle->waiting_cont)
	{
		if (2 * i + 2 < dongle->waiting_cont
			&& dongle->waiting_queue[2 * i + 2].deadline
			< dongle->waiting_queue[2 * i + 1].deadline)
			smallest = 2 * i + 2;
		else
			smallest = 2 * i + 1;
		if (dongle->waiting_queue[smallest].deadline
			>= dongle->waiting_queue[i].deadline)
			break ;
		tmp = dongle->waiting_queue[i];
		dongle->waiting_queue[i] = dongle->waiting_queue[smallest];
		dongle->waiting_queue[smallest] = tmp;
		i = smallest;
	}
}
