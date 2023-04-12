/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:45:24 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/12 21:31:18 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eating(t_philo	*philo)
{
	sem_wait(philo->data->forks);
	print_state(philo, TFR);
	sem_wait(philo->data->forks);
	print_state(philo, TFR);
	print_state(philo, EAT);
	sem_wait(philo->data->lock_death);
	philo->lt_eat = current_time();
	sem_post(philo->data->lock_death);
	philo_sleep(philo->data->t_eat);
	sem_wait(philo->data->lock_death);
	philo->n_eated++;
	sem_post(philo->data->lock_death);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	*check_death(void *args)
{
	t_philo	*philo;
	int		eated;

	philo = args;
	eated = 1;
	while (1)
	{
		sem_wait(philo->data->lock_death);
		if ((int)(current_time() - philo->lt_eat) >= philo->data->t_die)
		{
			sem_wait(philo->data->lock_print);
			printf("%lu %d %s\n", current_time() - \
			philo->data->t_start, philo->id + 1, DIE);
			exit(0);
		}
		else if (philo->data->n_eat > 0 && \
		philo->n_eated >= philo->data->n_eat && eated)
		{
			eated = 0;
			sem_post(philo->data->all_eat);
		}
		sem_post(philo->data->lock_death);
	}
	return (NULL);
}

int	action(t_philo *philo)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, check_death, philo))
		return (printf("Error: In pthread_create function\n"));
	while (1)
	{
		print_state(philo, TNK);
		eating(philo);
		print_state(philo, SLP);
		philo_sleep(philo->data->t_slp);
	}
	return (0);
}

static int	create_philos(t_philo *philo)
{
	pthread_t	th;
	int			i;

	i = -1;
	if (pthread_create(&th, NULL, check_all_eat, philo))
		return (printf("Error: In pthread_create of the main process"));
	while (++i < philo->data->n_philo)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
		{
			kill_processes(philo);
			philo->data->is_dead = 1;
			sem_post(philo->data->all_eat);
			return (printf("Error: In fork function\n"));
		}
		if (philo[i].pid == 0)
			if (action(&philo[i]) > 0)
				return (1);
	}
	ft_wait(philo);
	pthread_join(th, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philo;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Invalid arguments!!\n");
		return (1);
	}
	data = init_data(argc, argv);
	if (!data)
		return (2);
	philo = init_philo(data);
	if (!philo)
		return (3);
	if (create_philos(philo))
	{
		close_unlink_free(philo);
		return (4);
	}
	close_unlink_free(philo);
	return (0);
}
