/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmarani <pmarani@student.42firenze.it>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 19:12:57 by pmarani           #+#    #+#             */
/*   Updated: 2026/09/06 13:20:36 by pmarani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>

typedef struct s_coder	t_coder;
typedef struct s_data	t_data;

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

typedef struct s_waiter
{
	int		coder_id;
	long	deadline;

}	t_waiter;

typedef struct s_dongle
{
	int				state;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	long			last_release_time;
	t_waiter		*waiting_queue;
	int				waiting_cont;

}	t_dongle;

typedef struct s_coder
{
	pthread_t		thread;
	int				coder_number;
	int				total_compiled;
	long			last_start_compile;
	pthread_mutex_t	coder_mutex;
	t_params		*params;
	t_dongle		*left;
	t_dongle		*right;
	t_data			*data;

}	t_coder;

typedef struct s_data
{
	t_params		params;
	t_dongle		*dongles;
	t_coder			*coders;
	long			init_start_time;	
	int				is_simulation_over;
	pthread_mutex_t	is_simulation_over_mutex;
	pthread_mutex_t	log_mutex;
	pthread_t		*coder_threads;
	pthread_t		monitor_threads;

}	t_data;

int			init_simulation(int argc, char **argv, t_data *data);
int			validate_args(int argc, char **argv);
void		parse_params(char **argv, t_params *params);
void		create_coders(t_dongle *dongles, t_coder *coders,
				t_data *data, int number_of_coders);
int			create_dongles(t_dongle *dongles, int number_of_coders,
				int dongle_cooldown);
int			allocate_arrays(t_data *data);
long		get_current_time_ms(void);
void		release_dongle(t_dongle *dongle);
void		acquire_dongle(t_dongle *dongle, int coder_id,
				t_data *data, long deadline);
int			is_simulation_over(t_data *data);
void		set_simulation_over(t_data *data);
int			acquire_both_dongles(t_coder *coder);
void		remove_from_queue(t_dongle *dongle);
void		ft_usleep(long time_in_ms);
void		acquire_ordered(t_coder *coder);
void		heap_push(t_dongle *dongle, t_waiter waiter);
t_waiter	heap_pop(t_dongle *dongle);
void		sift_down(t_dongle *dongle, int i);
void		enqueue_coder(t_dongle *dongle, t_data *data,
				int coder_id, long deadline);
void		dequeue_coder(t_dongle *dongle, t_data *data);
void		*coder_routine(void *arg);
void		print_status(t_coder *coder, char *str);
void		*monitor_routine(void *args);
int			check_coders(t_data *data);
int			start_threads(t_data *data);
void		cleanup(t_data *data);
int			is_burned_out(t_data *data, int i);
int			is_higher_priority(t_waiter a, t_waiter b);
int			start_monitor(t_data *data);

#endif