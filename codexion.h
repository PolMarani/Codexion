/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 19:12:57 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/04 01:43:02 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

typedef struct s_params
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	int	scheduler;

}	t_params;

typedef struct s_dongle
{
	int				state;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				last_release_time;
	int				*waiting_queue;
	int				waiting_cont;

}	t_dongle;

typedef struct s_coder
{
	int			coder_number;
	int			total_compiled;
	int			last_start_compile;	
	t_params	*params;
	t_dongle	*left;
	t_dongle	*right;
	t_data		*data;

}	t_coder;

typedef struct s_data
{
	t_params		params;
	t_dongle		*dongles;
	t_coder			*coders;
	long			init_start_time;	
	int				is_simulation_over;
	pthread_mutex_t	is_simulation_over_mutex;

}	t_data;

int		init_simulation(int argc, char **argv, t_data *data);
int		validate_args(int argc, char **argv);
void	parse_params(char **argv, t_params *params);
void	create_coders(t_dongle *dongles, t_coder *coders,
			t_params *params, int number_of_coders);
void	create_dongles(t_dongle *dongles, int number_of_coders,
			int dongle_cooldown);
long	get_current_time_ms(void);
void	release_dongle(t_dongle *dongle);
void	acquire_dongle(t_dongle *dongle, int dongle_cooldown);
int		is_simulation_over(t_data *data);
void	set_simulation_over(t_data *data);
int	acquire_both_dongles(t_coder *coder);
void	remove_from_queue(t_dongle *dongle);

#endif