/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:49:58 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/11 04:45:09 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	init_sem(t_data *data)
{
	sem_unlink("sem_forks");
	sem_unlink("sem_lock_print");
	sem_unlink("sem_lock_death");
	sem_unlink("sem_all_eat");
	data->forks = sem_open("sem_forks", O_CREAT, 0644, data->n_philo);
	if (data->forks == SEM_FAILED)
		return (printf("Error: in sem_open function for forks\n"));
	data->all_eat = sem_open("sem_all_eat", O_CREAT, 0644, data->n_philo);
	if (data->all_eat == SEM_FAILED)
		return (printf("Error: in sem_open function for forks\n"));
	data->lock_print = sem_open("sem_lock_print", O_CREAT, 0644, 1);
	if (data->lock_print == SEM_FAILED)
		return (printf("Error: in sem_open function for lock_death\n"));
	data->lock_death = sem_open("sem_lock_death", O_CREAT, 0644, 1);
	if (data->lock_death == SEM_FAILED)
		return (printf("Error: in sem_open function for lock_death\n"));
	return (0);
}

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->n_philo = ft_atoi(argv[1]);
	data->t_start = current_time();
	data->t_die = ft_atoi(argv[2]);
	data->t_eat = ft_atoi(argv[3]);
	data->t_slp = ft_atoi(argv[4]);
	data->n_eat = 0;
	data->is_dead = 0;
	if (argc == 6)
		data->n_eat = ft_atoi(argv[5]);
	if (check_args(data, argc))
		return (free(data), NULL);
	if (init_sem(data))
		return (free(data), NULL);
	return (data);
}

t_philo	*init_philo(t_data *data)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * data->n_philo);
	if (!philo)
	{
		free(data);
		return (NULL);
	}
	i = -1;
	while (++i < data->n_philo)
	{
		philo[i].id = i;
		philo[i].pid = -1;
		philo[i].n_eated = 0;
		philo[i].lt_eat = current_time();
		philo[i].data = data;
	}
	return (philo);
}

void	close_unlink_free(t_philo *philo)
{
	sem_close(philo->data->forks);
	sem_close(philo->data->lock_print);
	sem_close(philo->data->lock_death);
	sem_close(philo->data->all_eat);
	sem_unlink("sem_forks");
	sem_unlink("sem_lock_print");
	sem_unlink("sem_lock_death");
	sem_unlink("sem_all_eat");
	free(philo->data);
	free(philo);
}
