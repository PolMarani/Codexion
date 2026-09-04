/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 20:39:04 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/04 20:53:17 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void    print_status(t_coder *coder, char *str)
{
    long    timestamp;

    pthread_mutex_lock(&coder->data->log_mutex);
    if (is_simulation_over(coder->data) == 0)
    {
        timestamp = get_current_time_ms() - coder->data->init_start_time;
        printf("%ld %d %s\n", timestamp, coder->coder_number, str);
    }
}
