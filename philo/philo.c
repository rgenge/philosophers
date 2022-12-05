/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/05 10:30:30 by acosta-a         ###   ########.fr       */
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
	usleep(10);
	pthread_mutex_destroy(&dt->print_output);
	i = -1;
	while (++i < dt->num_philo)
		pthread_mutex_destroy(&dt->forks[i]);
	free(dt->forks);
	free(dt->lock_dead);
	free(dt->lock_last);
	free(dt->lock_meals);
	free(dt->lock_state);
	free(dt->lock_forks);
	free(dt);
	free(philo);
}

int	fork_check(t_philo *philo, int right_fork)
{
	pthread_mutex_lock(philo->dt->lock_dead);
	if (philo->dt->num_philo == 1 || philo->dt->dead == DEAD)
	{
		pthread_mutex_unlock(philo->dt->lock_dead);
		pthread_mutex_unlock(philo->dt->lock_forks);
		pthread_mutex_unlock(&philo->dt->forks[philo->id]);
		return (0);
	}
	pthread_mutex_unlock(philo->dt->lock_dead);
	pthread_mutex_lock(&philo->dt->forks[right_fork]);
	put_screen(philo, RIGHT_FORK);
	return (1);
}

void	eat_sleep(t_philo *philo, int right_fork)
{
	put_screen(philo, EAT);
	pthread_mutex_lock(philo->dt->lock_state);
	philo->state = EAT;
	pthread_mutex_unlock(philo->dt->lock_state);
	pthread_mutex_lock(philo->dt->lock_meals);
	philo->meals_count++;
	pthread_mutex_unlock(philo->dt->lock_meals);
	pthread_mutex_lock(philo->dt->lock_last);
	philo->last_eat = get_time_now();
	pthread_mutex_unlock(philo->dt->lock_last);
	usleep(philo->dt->eat_time * 1000);
	put_screen(philo, SLEEP);
	pthread_mutex_lock(philo->dt->lock_state);
	philo->state = SLEEP;
	pthread_mutex_unlock(philo->dt->lock_state);
	pthread_mutex_unlock(&philo->dt->forks[philo->id]);
	pthread_mutex_unlock(&philo->dt->forks[right_fork]);
	usleep(philo->dt->sleep_time * 1000);
}

void	*philo_doing(void *data)
{
	t_philo					*philo;
	int						right_fork;

	philo = data;
	while ((philo->meals_count < philo->dt->n_must_eat
			|| philo->dt->n_must_eat == -1))
	{
		pthread_mutex_lock(philo->dt->lock_dead);
		if (philo->dt->dead != 0)
		{
			pthread_mutex_unlock(philo->dt->lock_dead);
			return (NULL);
		}
		pthread_mutex_unlock(philo->dt->lock_dead);
		right_fork = (philo->id + 1) % philo->dt->num_philo;
//		pthread_mutex_lock(philo->dt->lock_forks);
		pthread_mutex_lock(&philo->dt->forks[philo->id]);
		pthread_mutex_lock(philo->dt->lock_forks);
		put_screen(philo, LEFT_FORK);
		if (fork_check(philo, right_fork) == 0)
			return (NULL);
		pthread_mutex_unlock(philo->dt->lock_forks);
		eat_sleep(philo, right_fork);
		put_screen(philo, THINK);
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
