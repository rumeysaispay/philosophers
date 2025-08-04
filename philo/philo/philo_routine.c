/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:53:42 by rispay-s          #+#    #+#             */
/*   Updated: 2025/08/01 12:53:42 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	simulation_ended(t_data *data)
{
	return (safe_read_flag(data, 2));
}

static void	apply_timing_offset(t_philo *philo)
{
	if (philo->data->number_of_philo == 3)
	{
		if (philo->id == 3)
			ft_usleep_check(philo->data->time_to_eat - 50, philo->data);
	}
	else if (philo->id % 2 == 1)
		ft_usleep_check(philo->data->time_to_eat / 2, philo->data);
}

static int	handle_single_philo(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->number_of_philo == 1)
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		while (!simulation_ended(data))
			usleep(100);
		pthread_mutex_unlock(philo->l_fork);
		return (1);
	}
	return (0);
}

static void	init_philo_thread_and_wait(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->start_lock);
	data->threads_ready++;
	if (data->threads_ready == data->number_of_philo)
	{
		data->start_time = get_time();
	}
	pthread_mutex_unlock(&data->start_lock);
	while (1)
	{
		pthread_mutex_lock(&data->start_lock);
		if (data->threads_ready == data->number_of_philo)
		{
			pthread_mutex_unlock(&data->start_lock);
			break ;
		}
		pthread_mutex_unlock(&data->start_lock);
		usleep(100);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	init_philo_thread_and_wait(philo);
	safe_update_meal_data(philo, data->start_time, 0);
	if (handle_single_philo(philo))
		return (NULL);
	apply_timing_offset(philo);
	while (!simulation_ended(data) && should_continue_eating(philo))
	{
		if (take_forks(philo) == SUCCESS)
		{
			eat_and_release_forks(philo);
			sleep_and_think(philo);
		}
		else
			break ;
	}
	return (NULL);
}
