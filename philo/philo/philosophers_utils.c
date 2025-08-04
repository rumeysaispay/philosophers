/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:38:16 by rispay-s          #+#    #+#             */
/*   Updated: 2025/07/25 16:38:16 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>

long int	ft_atol(char *str)
{
	int			i;
	long long	result;

	result = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			e_message("Negative number you entered!");
			return (ERROR);
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (str[i] - '0') + (result * 10);
		i++;
	}
	return (result);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

int	is_valid_number(const char *str)
{
	if (!str || *str == '\0')
		return (ERROR);
	if (*str == '+')
		str++;
	if (*str == '\0')
		return (ERROR);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (ERROR);
		str++;
	}
	return (SUCCESS);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_status(t_philo *philo, char *message)
{
	long long	time;
	long long	start_time;
	int			should_print;

	if (!philo || !philo->data)
		return ;
	pthread_mutex_lock(&philo->data->death_lock);
	should_print = ((!philo->data->dead_flag && !philo->data->finish_flag)
			|| (ft_strncmp(message, "died", 4) == 0));
	pthread_mutex_unlock(&philo->data->death_lock);
	if (should_print)
	{
		pthread_mutex_lock(&philo->data->start_lock);
		start_time = philo->data->start_time;
		pthread_mutex_unlock(&philo->data->start_lock);
		pthread_mutex_lock(&philo->data->print_lock);
		time = get_time() - start_time;
		printf("%lld %d %s\n", time, philo->id, message);
		pthread_mutex_unlock(&philo->data->print_lock);
	}
}
