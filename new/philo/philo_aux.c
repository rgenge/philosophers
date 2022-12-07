/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:19 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/05 10:28:59 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death_aux(t_philo *philo, int i)
{
	long long	now;

	now = get_time_now();
	pthread_mutex_lock(philo->dt->lock_last);
	if (now - philo[i].last_eat >= philo->dt->die_time)
	{
		pthread_mutex_lock(philo->dt->lock_dead);
		philo->dt->dead = DEAD;
		pthread_mutex_unlock(philo->dt->lock_dead);
		put_screen(philo, DEAD);
		pthread_mutex_unlock(philo->dt->lock_last);
		return (0);
	}
	pthread_mutex_unlock(philo->dt->lock_last);
	return (1);
}

int	state_check(t_philo *philo, int i)
{
	pthread_mutex_lock(philo->dt->lock_meals);
	if (philo[i].meals_count >= philo->dt->n_must_eat
		&& philo->dt->n_must_eat != -1)
		philo->dt->all_eat++;
	pthread_mutex_unlock(philo->dt->lock_meals);
	pthread_mutex_lock(philo->dt->lock_state);
	if (philo[i].state != EAT)
	{
		pthread_mutex_unlock(philo->dt->lock_state);
		return (1);
	}
	else
		pthread_mutex_unlock(philo->dt->lock_state);
	return (0);
}

void	*check_death(void *arg)
{
	t_philo		*philo;
	int			i;

	philo = arg;
	usleep(100);
	while (philo->dt->dead != DEAD && philo->dt->all_eat < philo->dt->num_philo)
	{
		i = -1;
		while (++i < philo->dt->num_philo)
		{
			if (state_check(philo, i))
			{
				if (check_death_aux(philo, i) == 0)
					return (NULL);
				if (philo->dt->all_eat == philo->dt->num_philo)
					return (NULL);
			}
		}
		usleep(10);
	}
	return (NULL);
}
