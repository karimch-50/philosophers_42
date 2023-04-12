/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 11:45:27 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/11 03:43:25 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	eating(t_philo	*philo)
{
	pthread_mutex_lock(&philo->data->forks[(philo->id + 1) \
	% philo->data->n_philo]);
	print_state(philo, TFR);
	if (philo->data->n_philo == 1)
	{
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) \
		% philo->data->n_philo]);
		return (1);
	}
	pthread_mutex_lock(&philo->data->forks[philo->id]);
	print_state(philo, TFR);
	print_state(philo, EAT);
	pthread_mutex_lock(&philo->data->lock_death);
		philo->lt_eat = current_time();
	pthread_mutex_unlock(&philo->data->lock_death);
	philo_sleep(philo->data->t_eat, philo);
	pthread_mutex_lock(&philo->data->lock_death);
		philo->data->n_eated++;
	pthread_mutex_unlock(&philo->data->lock_death);
	pthread_mutex_unlock(&philo->data->forks[philo->id]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) \
	% philo->data->n_philo]);
	return (0);
}

void	*action(void *args)
{
	t_philo	*philo;

	philo = args;
	while (1)
	{
		pthread_mutex_lock(&philo->data->lock_death);
		if (philo->data->is_dead)
		{
			pthread_mutex_unlock(&philo->data->lock_death);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->lock_death);
		print_state(philo, TNK);
		if (eating(philo))
			return (NULL);
		print_state(philo, SLP);
		philo_sleep(philo->data->t_slp, philo);
	}
	return (NULL);
}

static int	create_philos(t_philo *philo)
{
	int	i;

	if (init_mutex(philo))
		return (1);
	i = 0;
	while (i < philo->data->n_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, action, &philo[i]))
			return (printf("Error: In pthread_create function\n"));
		i += 2;
		usleep(5);
	}
	usleep(200);
	i = 1;
	while (i < philo->data->n_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, action, &philo[i]))
			return (printf("Error: In pthread_create function\n"));
		i += 2;
	}
	check_death(philo);
	return (0);
}

static int	join_free_philos(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->data->n_philo)
		if (pthread_join(philo[i].thread, NULL))
			return (printf ("Error: In pthread_join function\n"));
	i = -1;
	while (++i < philo->data->n_philo)
		if (pthread_mutex_destroy(&philo->data->forks[i]))
			return (printf("Error: In pthread_mutex_destroy function: forks\n"));
	if (pthread_mutex_destroy(&philo->data->lock_print))
	{
		printf("Error: In pthread_mutex_destroy function: lock_print\n");
		return (1);
	}
	if (pthread_mutex_destroy(&philo->data->lock_death))
	{
		printf("Error: In pthread_mutex_destroy function: lock_death\n");
		return (1);
	}
	ft_free_philo(philo);
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
		return (4);
	if (create_philos(philo))
	{
		free(data->forks);
		return (free(data), free(philo), 5);
	}
	if (join_free_philos(philo))
	{
		free(data->forks);
		return (free(data), free(philo), 6);
	}
	return (0);
}
