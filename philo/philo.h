/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:34:28 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/08 19:48:12 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define TFR "has taken a fork"
# define EAT "is eating"
# define SLP "is sleeping"
# define TNK "is thinking"
# define DIE "died"

typedef struct s_data
{
	int				n_philo;
	int				n_eated;
	int				n_eat;
	int				is_dead;
	unsigned long	t_start;
	int				t_die;
	int				t_eat;
	int				t_slp;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock_print;
	pthread_mutex_t	lock_death;
}	t_data;

typedef struct s_philo
{
	int				id;
	unsigned long	lt_eat;
	pthread_t		thread;
	t_data			*data;
}	t_philo;

int				ft_atoi(const char *str);
t_data			*init_data(int argc, char **argv);
t_philo			*init_philo(t_data *data);
int				init_mutex(t_philo *philo);
unsigned long	current_time(void);
int				philo_sleep(unsigned long time, t_philo *philo);
void			print_state(t_philo *philo, char *msg);
void			check_death(t_philo *philo);
void			ft_free_philo(t_philo *philo);

#endif