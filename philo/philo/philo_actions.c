/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 12:00:00 by rispay-s          #+#    #+#             */
/*   Updated: 2025/08/01 13:21:33 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_continue_eating(t_philo *philo)
{
	int	eat_count;
	int	num_of_meals_be_eaten;

	safe_read_meal_data(philo, NULL, &eat_count);
	num_of_meals_be_eaten = philo->data->num_of_meals_be_eaten;
	return (num_of_meals_be_eaten == -1 || eat_count < num_of_meals_be_eaten);
}

static int	take_single_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	if (simulation_ended(philo->data))
	{
		pthread_mutex_unlock(fork);
		return (ERROR);
	}
	print_status(philo, "has taken a fork");
	return (SUCCESS);
}

int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (simulation_ended(philo->data))
		return (ERROR);
	if (philo->id % 2 == 0)
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	else
	{
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;
	}
	if (take_single_fork(philo, first_fork) == ERROR)
		return (ERROR);
	if (take_single_fork(philo, second_fork) == ERROR)
	{
		pthread_mutex_unlock(first_fork);
		return (ERROR);
	}
	return (SUCCESS);
}

void	eat_and_release_forks(t_philo *philo)
{
	print_status(philo, "is eating");
	safe_update_meal_data(philo, get_time(), 1);
	ft_usleep_check(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep_check(philo->data->time_to_sleep, philo->data);
	if (simulation_ended(philo->data))
		return ;
	print_status(philo, "is thinking");
	if (philo->data->number_of_philo % 2 == 1)
	{
		ft_usleep_check((philo->data->time_to_eat * 2)
			- philo->data->time_to_sleep, philo->data);
	}
}
