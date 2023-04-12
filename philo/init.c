/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 15:58:25 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/11 03:43:02 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_args(t_data *data, int ac)
{
	if (data->n_philo <= 0)
		return (printf("Error: Wrong number of philo\n"));
	else if (data->t_die <= 0)
		return (printf("Error: Wrong time to die\n"));
	else if (data->t_eat <= 0)
		return (printf("Error: Wrong time to eat\n"));
	else if (data->t_slp <= 0)
		return (printf("Error: Wrong time to sleep\n"));
	else if (ac == 6 && data->n_eat <= 0)
		return (printf("Error: Wrong number of times \
each philosopher must eat\n"));
	else
		return (0);
}

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->n_philo = ft_atoi(argv[1]);
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	data->n_eat = 0;
	data->n_eated = 0;
	if (argc == 6)
		data->n_eat = ft_atoi(argv[5]);
	if (check_args(data, argc))
		return (free(data), NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->forks)
		return (free(data), NULL);
	data->is_dead = 0;
	data->t_start = current_time();
	return (data);
}

t_philo	*init_philo(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!philo)
	{
		free(data->forks);
		free(data);
		return (NULL);
	}
	i = -1;
	while (++i < data->n_philo)
	{
		philo[i].id = i;
		philo[i].lt_eat = current_time();
		philo[i].data = data;
	}
	return (philo);
}

int	init_mutex(t_philo *philo)
{
	int	i;

	if (pthread_mutex_init(&philo->data->lock_print, NULL))
		return (printf("Error: In pthread_mutex_init function of lock_print\n"));
	if (pthread_mutex_init(&philo->data->lock_death, NULL))
		return (printf("Error: In pthread_mutex_init function of lock_death\n"));
	i = -1;
	while (++i < philo->data->n_philo)
	{
		if (pthread_mutex_init(&philo->data->forks[i], NULL))
			return (printf("Error: In pthread_mutex_init function\n"));
	}
	return (0);
}

void	ft_free_philo(t_philo *philo)
{
	free(philo->data->forks);
	free(philo->data);
	free(philo);
}
