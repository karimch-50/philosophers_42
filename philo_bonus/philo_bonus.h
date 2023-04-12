/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:42:09 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/12 18:27:49 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <semaphore.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/time.h>

# define TFR "has taken a fork"
# define EAT "is eating"
# define SLP "is sleeping"
# define TNK "is thinking"
# define DIE "died"

typedef struct s_data
{
	int				n_philo;
	unsigned long	t_start;
	int				is_dead;
	int				n_eat;
	int				t_die;
	int				t_eat;
	int				t_slp;
	sem_t			*forks;
	sem_t			*lock_print;
	sem_t			*lock_death;
	sem_t			*all_eat;
}	t_data;

typedef struct s_philo
{
	int				id;
	pid_t			pid;
	pid_t			killed_ps;
	unsigned long	lt_eat;
	int				n_eated;
	t_data			*data;
}	t_philo;

int				ft_atoi(const char *str);
t_data			*init_data(int argc, char **argv);
t_philo			*init_philo(t_data *data);
unsigned long	current_time(void);
int				philo_sleep(unsigned long time);
void			print_state(t_philo *philo, char *msg);
void			*check_all_eat(void *args);
void			kill_processes(t_philo *philo);
void			ft_wait(t_philo *philo);
void			close_unlink_free(t_philo *philo);

#endif