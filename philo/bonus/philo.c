/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/11/27 17:42:40 by acosta-a         ###   ########.fr       */
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
	dt->i = -1;
	free(dt->forks);
	free(dt);
	free(philo);
}

int	fork_check(t_philo *philo)
{
	if (philo->dt->num_philo == 1 || philo->dt->dead == DEAD)
	{
		sem_post(philo->dt->forks);
		return (0);
	}
	sem_wait(philo->dt->forks);
	put_screen(philo, RIGHT_FORK);
	return (1);
}

void	*philo_doing(void *data)
{
	t_philo					*philo;

	philo = data;
	while ((philo->meals_count < philo->dt->n_must_eat
			|| philo->dt->n_must_eat == -1) && philo->dt->dead == 0)
	{
		sem_wait(philo->dt->forks);
		put_screen(philo, LEFT_FORK);
		if (fork_check(philo) == 0)
			return (NULL);
		put_screen(philo, EAT);
		philo->state = EAT;
		philo->meals_count++;
		philo->last_eat = get_time_now();
		usleep(philo->dt->eat_time * 1000);
		sem_post(philo->dt->forks);
		sem_post(philo->dt->forks);
		put_screen(philo, SLEEP);
		philo->state = SLEEP;
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
					exit(1);
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
/*	while (++dt->i < dt->num_philo)
	{
		philo[dt->i].last_eat = get_time_now();
		pthread_create(&philo[dt->i].t, NULL, philo_doing, &philo[dt->i]);
		usleep(50);
	}*/
	while (++dt->i < dt->num_philo)
	{
		philo[dt->i].pid = fork();
		if (philo[dt->i].pid == 0)
		{
			philo[dt->i].last_eat = get_time_now();
			pthread_create(&philo[dt->i].t, NULL, philo_doing, &philo[dt->i]);
		}
		philo[dt->i].last_eat = get_time_now();
		usleep(50);
	}
	waitpid(&pid, 0, NULL);
	pthread_create(&monitor, NULL, &check_death, philo);
	usleep(1000);
	philo->dt->i = -1;
	while (++philo->dt->i < philo->dt->num_philo)
	{
		pthread_join(philo[philo->dt->i].t, NULL);
		exit(1);
	}
	pthread_join(monitor, NULL);
}
