/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 23:18:14 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/12 21:15:53 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*check_all_eat(void *args)
{
	t_philo	*philo;
	int		i;

	philo = args;
	i = -1;
	while (++i < philo->data->n_philo)
		sem_wait(philo->data->all_eat);
	i = -1;
	while (++i < philo->data->n_philo)
	{
		if (philo->data->is_dead)
			return (NULL);
		sem_wait(philo->data->all_eat);
	}
	kill(philo[0].pid, SIGKILL);
	philo[0].pid = -1;
	return (NULL);
}

void	kill_processes(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->data->n_philo)
	{
		if (philo[i].pid != -1 && philo[i].pid != philo->killed_ps)
		{
			if (kill(philo[i].pid, SIGKILL) != 0)
				printf("Error: In kill function\n");
		}
	}
}
