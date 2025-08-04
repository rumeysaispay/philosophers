/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_philo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:37:06 by rispay-s          #+#    #+#             */
/*   Updated: 2025/07/25 16:37:06 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	cleanup_threads(t_data *data, int created_count)
{
	int	i;

	pthread_mutex_lock(&data->death_lock);
	data->dead_flag = 1;
	pthread_mutex_unlock(&data->death_lock);
	i = 0;
	while (i < created_count)
	{
		pthread_join(data->philo[i].thread_id, NULL);
		i++;
	}
}

void	free_resources(t_data *data)
{
	int	i;

	if (!data)
		return ;
	pthread_mutex_lock(&data->death_lock);
	data->dead_flag = 1;
	pthread_mutex_unlock(&data->death_lock);
	if (data->forks)
	{
		i = 0;
		while (i < data->number_of_philo)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philo)
	{
		free(data->philo);
		data->philo = NULL;
	}
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->start_lock);
}

void	e_message(char *str)
{
	printf("%s", str);
}
