/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 13:17:41 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/02 17:21:42 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>

long	get_current_time_ms(void)
{
	struct timeval	ms_of_starttime;

	gettimeofday(&ms_of_starttime, NULL);
	return (ms_of_starttime.tv_sec * 1000 + ms_of_starttime.tv_usec / 1000);
}
