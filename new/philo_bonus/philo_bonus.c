/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/08 21:02:57 by acosta-a         ###   ########.fr       */
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

void	*philo_doing(void *data)
{
	t_philo					*philo;

	philo = data;
	while ((philo->meals_count < philo->dt->n_must_eat
			|| philo->dt->n_must_eat == -1))
	{
		sem_wait(philo->dt->lock_dead);
		if (philo->dt->dead != 0)
			return (NULL);
		sem_post(philo->dt->lock_dead);
		sem_wait(philo->dt->lock_forks);
		sem_wait(philo->dt->forks);
		put_screen(philo, LEFT_FORK);
		if (fork_check(philo) == 0)
			return (NULL);
		eat_sleep(philo);
	}
	return (NULL);
}

void	check_death_aux(t_philo *philo)
{
	long long	now;

	sem_post(philo->dt->lock_state);
	now = get_time_now();
	if (now - philo->last_eat >= philo->dt->die_time)
	{
		sem_wait(philo->dt->lock_dead);
		philo->dt->dead = DEAD;
		sem_post(philo->dt->lock_dead);
		put_screen(philo, DEAD);
		exit_free_close_2(philo);
	}
	sem_wait(philo->dt->lock_meals);
}

void	*check_death(void *arg)
{
	t_philo		*philo;

	philo = arg;
	while (1)
	{
		sem_wait(philo->dt->lock_state);
		if (philo->state != EAT)
		{
			check_death_aux(philo);
			if (philo->meals_count >= philo->dt->n_must_eat
				&& philo->dt->n_must_eat != -1)
				philo->dt->all_eat++;
			sem_post(philo->dt->lock_meals);
			sem_wait(philo->dt->lock_meals);
			if (philo->dt->all_eat == philo->dt->num_philo)
				exit_free_close_3(philo);
			sem_post(philo->dt->lock_meals);
		}
		sem_post(philo->dt->lock_state);
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
