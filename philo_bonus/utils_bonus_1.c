/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 14:52:28 by kchaouki          #+#    #+#             */
/*   Updated: 2023/04/12 21:33:12 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	philo_sleep(unsigned long time)
{
	unsigned long	start;

	start = current_time();
	while (current_time() - start < time)
		usleep(200);
	return (0);
}

void	print_state(t_philo *philo, char *msg)
{
	sem_wait(philo->data->lock_print);
	printf("%lu %d %s\n", current_time() - \
	philo->data->t_start, philo->id + 1, msg);
	sem_post(philo->data->lock_print);
}

void	ft_wait(t_philo *philo)
{
	int	status_code;

	status_code = 0;
	philo->killed_ps = waitpid(-1, &status_code, 0);
	kill_processes(philo);
	philo->data->is_dead = 1;
	if ((status_code >> 8) > 0)
	{
		sem_post(philo->data->all_eat);
		return ;
	}
	sem_post(philo->data->all_eat);
}
