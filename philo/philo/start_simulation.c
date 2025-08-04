/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:56:47 by rispay-s          #+#    #+#             */
/*   Updated: 2025/08/01 13:56:47 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_usleep_check(size_t miliseconds, t_data *data)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < miliseconds)
	{
		if (data && simulation_ended(data))
			return (1);
		usleep(50);
	}
	return (0);
}

static int	create_monitor(t_data *data, pthread_t *monitor_thread)
{
	if (pthread_create(monitor_thread, NULL, monitor_philos, data) != SUCCESS)
	{
		cleanup_threads(data, data->number_of_philo);
		e_message("Error: Monitor thread could not be created!");
		free_resources(data);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	create_philo(t_data *data)
{
	int	i;

	data->philo = malloc(sizeof(t_philo) * data->number_of_philo);
	if (!data->philo)
		return (ERROR);
	data->threads_ready = 0;
	i = 0;
	while (i < data->number_of_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].eat_count = 0;
		data->philo[i].last_meal_time = 0;
		data->philo[i].data = data;
		data->philo[i].l_fork = &data->forks[i];
		data->philo[i].r_fork = &data->forks[(i + 1) % data->number_of_philo];
		if (pthread_create(&data->philo[i].thread_id, NULL, philo_routine,
				&data->philo[i]) != SUCCESS)
		{
			cleanup_threads(data, i);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philo);
	if (!data->forks)
		return (ERROR);
	i = 0;
	while (i < data->number_of_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != SUCCESS)
		{
			free_resources(data);
			return (ERROR);
		}
		i++;
	}
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->meal_lock, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->start_lock, NULL) != 0)
		return (ERROR);
	return (SUCCESS);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	if (init_mutexes(data) == ERROR)
	{
		e_message("Error: Mutexes could not be created!");
		return (ERROR);
	}
	if (create_philo(data) == ERROR)
	{
		e_message("Error: Philosopher threads could not be created!");
		free_resources(data);
		return (ERROR);
	}
	if (create_monitor(data, &monitor_thread) == ERROR)
		return (ERROR);
	i = 0;
	while (i < data->number_of_philo)
		pthread_join(data->philo[i++].thread_id, NULL);
	pthread_join(monitor_thread, NULL);
	return (SUCCESS);
}
