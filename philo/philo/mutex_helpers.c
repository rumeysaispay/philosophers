/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:37:40 by rispay-s          #+#    #+#             */
/*   Updated: 2025/08/01 13:20:40 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	safe_read_flag(t_data *data, int flag_type)
{
	int	result;

	pthread_mutex_lock(&data->death_lock);
	if (flag_type == 0)
		result = data->dead_flag;
	else if (flag_type == 1)
		result = data->finish_flag;
	else
		result = (data->dead_flag || data->finish_flag);
	pthread_mutex_unlock(&data->death_lock);
	return (result);
}

void	safe_set_flag(t_data *data, int flag_type, int value)
{
	pthread_mutex_lock(&data->death_lock);
	if (flag_type == 0)
		data->dead_flag = value;
	else if (flag_type == 1)
		data->finish_flag = value;
	pthread_mutex_unlock(&data->death_lock);
}

void	safe_read_meal_data(t_philo *philo, long long *last_meal,
		int *eat_count)
{
	pthread_mutex_lock(&philo->data->meal_lock);
	if (last_meal)
		*last_meal = philo->last_meal_time;
	if (eat_count)
		*eat_count = philo->eat_count;
	pthread_mutex_unlock(&philo->data->meal_lock);
}

void	safe_update_meal_data(t_philo *philo, long long new_meal_time,
		int increment_count)
{
	pthread_mutex_lock(&philo->data->meal_lock);
	if (new_meal_time != -1)
		philo->last_meal_time = new_meal_time;
	if (increment_count)
		philo->eat_count++;
	pthread_mutex_unlock(&philo->data->meal_lock);
}
