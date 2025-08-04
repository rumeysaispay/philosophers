/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_philos.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:53:33 by rispay-s          #+#    #+#             */
/*   Updated: 2025/08/01 12:53:33 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static int	check_all_ate(t_data *data)
{
	int	i;
	int	ate_enough;
	int	eat_count;

	if (data->num_of_meals_be_eaten == -1)
		return (0);
	i = 0;
	ate_enough = 0;
	while (i < data->number_of_philo)
	{
		safe_read_meal_data(&data->philo[i], NULL, &eat_count);
		if (eat_count >= data->num_of_meals_be_eaten)
			ate_enough++;
		i++;
	}
	if (ate_enough == data->number_of_philo)
	{
		safe_set_flag(data, 1, 1);
		return (1);
	}
	return (0);
}

static int	has_died(t_data *data, int i)
{
	long long	last_meal;
	long long	start_time;

	safe_read_meal_data(&data->philo[i], &last_meal, NULL);
	pthread_mutex_lock(&data->start_lock);
	start_time = data->start_time;
	pthread_mutex_unlock(&data->start_lock);
	if (start_time == 0 || last_meal == 0)
		return (SUCCESS);
	if (get_time() - last_meal >= data->time_to_die)
	{
		if (!safe_read_flag(data, 0))
		{
			safe_set_flag(data, 0, 1);
			safe_set_flag(data, 1, 1);
			ft_usleep_check(1, data);
			pthread_mutex_lock(&data->print_lock);
			printf("%lld %d died 💀\n", get_time() - start_time,
				data->philo[i].id);
			pthread_mutex_unlock(&data->print_lock);
		}
		return (ERROR);
	}
	return (SUCCESS);
}

static int	are_all_threads_ready(t_data *data)
{
	int	ready;

	pthread_mutex_lock(&data->start_lock);
	ready = (data->threads_ready >= data->number_of_philo);
	pthread_mutex_unlock(&data->start_lock);
	return (ready);
}

void	*monitor_philos(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (!are_all_threads_ready(data))
		ft_usleep_check(1, data);
	ft_usleep_check(50, data);
	while (!safe_read_flag(data, 2))
	{
		i = -1;
		while (++i < data->number_of_philo)
		{
			if (has_died(data, i))
				return (NULL);
			if (check_all_ate(data))
				return (NULL);
		}
		ft_usleep_check(1, data);
	}
	return (NULL);
}
