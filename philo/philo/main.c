/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:37:18 by rispay-s          #+#    #+#             */
/*   Updated: 2025/07/25 16:37:18 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

static int	check_args(t_data *data)
{
	if (data->number_of_philo <= 0 || data->number_of_philo > 200)
	{
		e_message("Arguments are not in the desired range!\n");
		return (ERROR);
	}
	if ((data->time_to_die <= 0 || data->time_to_die > INT_MAX)
		|| (data->time_to_eat <= 0 || data->time_to_eat > INT_MAX))
	{
		e_message("Arguments are not in the desired range!\n");
		return (ERROR);
	}
	if (data->time_to_sleep <= 0 || data->time_to_sleep > INT_MAX)
	{
		e_message("Arguments are not in the desired range!\n");
		return (ERROR);
	}
	if (data->num_of_meals_be_eaten == 0)
	{
		e_message("Arguments are not in the desired range!\n");
		return (ERROR);
	}
	return (SUCCESS);
}

static int	parse_args(int ac, char **av, t_data *data)
{
	int	i;

	i = 1;
	data->num_of_meals_be_eaten = -1;
	while (i < ac)
	{
		if (is_valid_number(av[i]) == ERROR)
		{
			e_message("Invalid character in arguments!\n");
			return (ERROR);
		}
		i++;
	}
	data->number_of_philo = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (ac == 6)
		data->num_of_meals_be_eaten = ft_atol(av[5]);
	return (SUCCESS);
}

static int	setup_dining(t_data *data, int ac, char **av)
{
	memset(data, 0, sizeof(t_data));
	data->dead_flag = 0;
	data->finish_flag = 0;
	data->threads_ready = 0;
	data->forks = NULL;
	data->philo = NULL;
	if (parse_args(ac, av, data) == ERROR)
		return (ERROR);
	if (check_args(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
	{
		e_message("Error: Invalid number of arguments!\n");
		return (ERROR);
	}
	if (setup_dining(&data, ac, av) == ERROR)
		return (ERROR);
	if (start_simulation(&data) == ERROR)
	{
		free_resources(&data);
		return (ERROR);
	}
	free_resources(&data);
	return (SUCCESS);
}
