/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/11/25 01:14:26 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_dt	*dt;
	t_philo	*philo;

	dt = (t_dt *)malloc(sizeof(t_dt));
	if (init(dt, &philo, argc, argv))
		return (ERROR);
	dt->start = get_time_now();
	philo->dt->dead = 0;
	create_philos(philo, dt);
	usleep(500);
	pthread_mutex_destroy(&dt->print_output);
	dt->i = -1;
	while (++dt->i < dt->num_philo)
		pthread_mutex_destroy(&dt->forks[dt->i]);
	free(dt->forks);
	free(dt);
	free(philo);
}

int	fork_check(t_philo *philo, int right_fork)
{
	if (philo->dt->num_philo == 1 || philo->dt->dead == DEAD)
	{
		pthread_mutex_unlock(&philo->dt->forks[philo->id]);
		return (0);
	}
	pthread_mutex_lock(&philo->dt->forks[right_fork]);
	put_screen(philo, RIGHT_FORK);
	return (1);
}

void	*philo_doing(void *data)
{
	t_philo					*philo;
	int						right_fork;

	philo = data;
	while ((philo->meals_count < philo->dt->n_must_eat
			|| philo->dt->n_must_eat == -1) && philo->dt->dead == 0)
	{
		right_fork = (philo->id + 1) % philo->dt->num_philo;
		pthread_mutex_lock(&philo->dt->forks[philo->id]);
		put_screen(philo, LEFT_FORK);
		if (fork_check(philo, right_fork) == 0)
			return (NULL);
		put_screen(philo, EAT);
		philo->state = EAT;
		philo->meals_count++;
		philo->last_eat = get_time_now();
		usleep(philo->dt->eat_time * 1000);
		put_screen(philo, SLEEP);
		philo->state = SLEEP;
		pthread_mutex_unlock(&philo->dt->forks[philo->id]);
		pthread_mutex_unlock(&philo->dt->forks[right_fork]);
		usleep(philo->dt->sleep_time * 1000);
		put_screen(philo, THINK);
	}
	return (NULL);
}

void	*check_death(void *arg)
{
	t_philo		*philo;
	long long	now;

	philo = arg;
	while (philo->dt->dead != DEAD && philo->dt->all_eat < philo->dt->num_philo)
	{
		philo->dt->i = -1;
		while (++philo->dt->i < philo->dt->num_philo)
		{
			if (philo[philo->dt->i].state != EAT)
			{
				now = get_time_now();
				if (now - philo[philo->dt->i].last_eat >= philo->dt->die_time)
				{
					philo->dt->dead = DEAD;
					put_screen(philo, DEAD);
					return (NULL);
				}
				if (philo[philo->dt->i].meals_count >= philo->dt->n_must_eat
					&& philo->dt->n_must_eat != -1)
					philo->dt->all_eat++;
			}
		}
	}
	return (NULL);
}

void	create_philos(t_philo *philo, t_dt *dt)
{
	pthread_t	monitor;

	dt->i = -1;
	while (++dt->i < dt->num_philo)
	{
		philo[dt->i].last_eat = get_time_now();
		pthread_create(&philo[dt->i].t, NULL, philo_doing, &philo[dt->i]);
		usleep(50);
	}
	pthread_create(&monitor, NULL, &check_death, philo);
	usleep(1000);
	philo->dt->i = -1;
	while (++philo->dt->i < philo->dt->num_philo)
		pthread_join(philo[philo->dt->i].t, NULL);
	pthread_join(monitor, NULL);
}
