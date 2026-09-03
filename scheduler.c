/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 01:41:50 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/04 01:44:11 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	remove_from_queue(t_dongle *dongle)
{
	int 	i;

	i = 0;
	while (i < dongle->waiting_cont - 1)
	{
		dongle->waiting_queue[i] = dongle->waiting_queue[i + 1];
		i++;
	}
	dongle->waiting_cont--;
}
