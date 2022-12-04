/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/03 03:22:10 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		sem_wait(philo->dt->lock_forks);
		sem_wait(philo->dt->forks);
		put_screen(philo, LEFT_FORK);
		if (fork_check(philo) == 0)
			return (NULL);
		sem_post(philo->dt->lock_forks);
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
	while (1)
	{
		if (philo->state != EAT)
		{
			now = get_time_now();
			if (now - philo->last_eat >= philo->dt->die_time)
			{
				philo->dt->dead = DEAD;
				put_screen(philo, DEAD);
				exit_free_close_2(philo);
			}
			if (philo->meals_count >= philo->dt->n_must_eat
				&& philo->dt->n_must_eat != -1)
			philo->dt->all_eat++;
			if (philo->meals_count == philo->dt->n_must_eat)
				exit_free_close_2(philo);
		}
		usleep(200);
	}
	return (NULL);
}

void	create_philos(t_philo *philo, t_dt *dt)
{
	int	x;

	dt->i = -1;
	while (++dt->i < dt->num_philo)
	{
		philo[dt->i].pid = fork();
		if (philo[dt->i].pid == 0)
		{
			pthread_create(&philo[dt->i].t, NULL, philo_doing, &philo[dt->i]);
			check_death(&philo[dt->i]);
		}
		philo[dt->i].last_eat = get_time_now();
		usleep(50);
	}
	while (waitpid(-1, &x, 0) >= 0)
	{
		if (WIFEXITED(x) && WEXITSTATUS(x) == 1)
		{
			kill_process(philo);
			exit_free_close(philo);
		}
	}
	usleep(100);
}
