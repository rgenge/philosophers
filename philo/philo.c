/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/03 11:39:39 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	int		i;
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
	i = -1;
	while (++i < dt->num_philo)
		pthread_mutex_destroy(&dt->forks[i]);
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
//		pthread_mutex_lock(philo->dt->lock_race2);
		philo->meals_count++;
//		pthread_mutex_unlock(philo->dt->lock_race2);
		pthread_mutex_lock(philo->dt->lock_race);
		philo->last_eat = get_time_now();
		pthread_mutex_unlock(philo->dt->lock_race);
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

int 	check_death_aux(t_philo *philo, int i)
{
	long long	now;
	now = get_time_now();
//	pthread_mutex_lock(philo->dt->lock_race3);
	if (now - philo[i].last_eat >= philo->dt->die_time)
	{
		philo->dt->dead = DEAD;
		put_screen(philo, DEAD);
//		pthread_mutex_unlock(philo->dt->lock_race3);
		return (0);
	}
//	pthread_mutex_unlock(philo->dt->lock_race3);
	return (1);
}
void	*check_death(void *arg)
{
	t_philo		*philo;
	int			i;

	philo = arg;
	usleep(200);
	while (philo->dt->dead != DEAD && philo->dt->all_eat < philo->dt->num_philo)
	{
		i = -1;
		while (++i < philo->dt->num_philo)
		{
			if (philo[i].state != EAT)
			{
				if(check_death_aux(philo, i) == 0)
					return (NULL);
				if (philo[i].meals_count >= philo->dt->n_must_eat
					&& philo->dt->n_must_eat != -1)
					philo->dt->all_eat++;
				if (philo->dt->all_eat == philo->dt->num_philo)
					return (NULL);
			}
		}
		usleep(10);
	}
	return (NULL);
}

void	create_philos(t_philo *philo, t_dt *dt)
{
	pthread_t	monitor;
	int			i;

	i = -1;
	while (++i < dt->num_philo)
	{
		philo[i].last_eat = get_time_now();
		pthread_create(&philo[i].t, NULL, philo_doing, &philo[i]);
		usleep(100);
	}
	pthread_create(&monitor, NULL, &check_death, philo);
	usleep(200);
	i = -1;
	while (++i < philo->dt->num_philo)
		pthread_join(philo[i].t, NULL);
	pthread_join(monitor, NULL);
}
