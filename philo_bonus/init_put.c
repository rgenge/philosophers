/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_put.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:22 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/03 03:19:57 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	digit_checker(int argc, char *argv[], t_philo *philo, t_dt *dt)
{
	int		i;

	i = 0;
	while (++i < argc)
	{
		if (argv[i][0] < '0' || argv[i][0] > '9')
		{
			ft_putstr_fd("ERR :Arguments must be digit > 0\n", STDERR);
			exit_free(dt, philo, 0);
		}
	}
	if ((argc == 6 && dt->n_must_eat <= 0) || (dt->num_philo < 0
			|| dt->die_time < 0 || dt->eat_time < 0 || dt->sleep_time < 0))
	{
		ft_putstr_fd("ERR :Arguments must be > 0\n", STDERR);
		exit_free(dt, philo, 0);
	}
}

int	init(t_dt *dt, t_philo **philo, int argc, char *argv[])
{
	if (argc > 6 || argc <= 4)
	{
		ft_putstr_fd("ERR :Type 5 or 6 arguments\n", STDERR);
		exit_free(dt, *philo, 1);
	}
	dt->num_philo = ft_atoi(argv[1]);
	dt->die_time = ft_atoi(argv[2]);
	dt->eat_time = ft_atoi(argv[3]);
	dt->sleep_time = ft_atoi(argv[4]);
	dt->n_must_eat = -1;
	if (argc == 6)
		dt->n_must_eat = ft_atoi(argv[5]);
	dt->all_eat = 0;
	*philo = (t_philo *)malloc(sizeof(t_philo) * dt->num_philo);
	digit_checker(argc, argv, *philo, dt);
	if (dt->num_philo == 0)
		exit_free(dt, *philo, 0);
	init_philos(dt, *philo);
	return (0);
}

void	init_philos(t_dt *dt, t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < dt->num_philo)
	{
		philo[i].id = i;
		philo[i].meals_count = 0;
		philo[i].dt = dt;
		philo[i].state = START;
		philo[i].last_eat = get_time_now();
	}
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("lock");
	dt->forks = sem_open("forks", O_CREAT, 0644, dt->num_philo);
	dt->print_output = sem_open("print", O_CREAT, 0644, 1);
	dt->lock_forks = sem_open("lock", O_CREAT, 0644, 1);
}

void	put_screen(t_philo *philo, int doing)
{
	long long	now;

	sem_wait(philo->dt->print_output);
	now = get_time_now();
	if ((doing == LEFT_FORK || doing == RIGHT_FORK) && philo->dt->dead == 0)
		printf("%llu %d has taken a fork \n", now, philo->id + 1);
	else if (doing == THINK && philo->dt->dead == 0)
		printf("%llu %d is thinking \n", now, philo->id + 1);
	else if (doing == EAT && philo->dt->dead == 0)
		printf("%llu %d is eating \n", now, philo->id + 1);
	else if (doing == SLEEP && philo->dt->dead == 0)
		printf("%llu %d is sleeping \n", now, philo->id + 1);
	else if (doing == DEAD)
		printf("%llu %d died \n", now, philo->id + 1);
	if (doing != DEAD)
		sem_post(philo->dt->print_output);
}
