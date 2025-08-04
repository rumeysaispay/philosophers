/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rispay-s <rispay-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 16:38:08 by rispay-s          #+#    #+#             */
/*   Updated: 2025/07/25 16:38:08 by rispay-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stddef.h>

# define SUCCESS 0
# define ERROR 1
# define INT_MAX 2147483647

typedef struct s_data
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	start_lock;
	struct s_philo	*philo;
	int				number_of_philo;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				num_of_meals_be_eaten;
	long long		start_time;
	int				finish_flag;
	int				dead_flag;
	int				threads_ready;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	t_data			*data;
}	t_philo;

void		free_resources(t_data *data);
int			start_simulation(t_data *data);
void		e_message(char *str);
void		*monitor_philos(void *arg);
void		*philo_routine(void *arg);
void		print_status(t_philo *philo, char *message);
long int	ft_atol(char *str);
long long	get_time(void);
void		cleanup_threads(t_data *data, int created_count);
int			is_valid_number(const char *str);
int			ft_usleep_check(size_t miliseconds, t_data *data);

int			safe_read_flag(t_data *data, int flag_type);
void		safe_set_flag(t_data *data, int flag_type, int value);
void		safe_read_meal_data(t_philo *philo, long long *last_meal,
				int *eat_count);
void		safe_update_meal_data(t_philo *philo, long long new_meal_time,
				int increment_count);

int			simulation_ended(t_data *data);
int			should_continue_eating(t_philo *philo);
int			take_forks(t_philo *philo);
void		eat_and_release_forks(t_philo *philo);
void		sleep_and_think(t_philo *philo);

#endif
