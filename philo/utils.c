/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:30:32 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/11 03:43:38 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	sign;
	long	ret;
	int		i;

	ret = ((sign = 1), (i = 0), 0);
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\n' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		if (ret > 2147483647)
			return (-1);
		ret = ret * 10 + str[i++] - '0';
	}
	if (str[i])
		return (-1);
	return (ret * sign);
}

unsigned long	current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: on gettimeofday function\n");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	philo_sleep(unsigned long time, t_philo *philo)
{
	unsigned long	start;

	start = current_time();
	while (current_time() - start < time)
	{
		pthread_mutex_lock(&philo->data->lock_death);
		if (philo->data->is_dead == 1)
		{
			pthread_mutex_unlock(&philo->data->lock_death);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->lock_death);
		usleep(200);
	}
	return (0);
}

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->lock_death);
	if (!philo->data->is_dead)
	{
		pthread_mutex_lock(&philo->data->lock_print);
		printf("%lu %d %s\n", current_time() - \
		philo->data->t_start, philo->id + 1, msg);
		pthread_mutex_unlock(&philo->data->lock_print);
	}
	pthread_mutex_unlock(&philo->data->lock_death);
}

void	check_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (1)
	{
		pthread_mutex_lock(&philo->data->lock_death);
		if ((int)(current_time() - philo[i].lt_eat) >= philo[i].data->t_die)
		{
			philo[i].data->is_dead = 1;
			printf("%lu %d %s\n", current_time() - \
			philo->data->t_start, philo[i].id + 1, DIE);
			pthread_mutex_unlock(&philo->data->lock_death);
			return ;
		}
		if (philo[i].data->n_eat > 0 && philo[i].data->n_eated > \
		philo[i].data->n_eat * philo[i].data->n_philo)
		{
			philo[i].data->is_dead = 1;
			pthread_mutex_unlock(&philo->data->lock_death);
			return ;
		}
		i = (i + 1) % philo->data->n_philo;
		pthread_mutex_unlock(&philo->data->lock_death);
	}
}
