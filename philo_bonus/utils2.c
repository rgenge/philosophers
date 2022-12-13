/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:24 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/11 17:02:16 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_process(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->dt->num_philo)
		kill(philo[i].pid, SIGKILL);
}

void	exit_free(t_dt *dt, t_philo *philo, int flag)
{
	if (flag == 0)
	{
		free(dt->forks);
		free(philo);
	}
	free(dt);
	exit (1);
}

void	exit_free_close(t_philo *philo)
{
	sem_close(philo->dt->forks);
	sem_close(philo->dt->lock_forks);
	sem_close(philo->dt->print_output);
	sem_close(philo->dt->lock_meals);
	sem_close(philo->dt->lock_state);
	sem_close(philo->dt->lock_dead);
	sem_close(philo->dt->lock_last);
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("print");
	sem_unlink("meals");
	sem_unlink("state");
	sem_unlink("dead");
	sem_unlink("last");
	free(philo->dt);
	free(philo);
}

void	exit_free_close_2(t_philo *philo)
{
	sem_post(philo->dt->lock_meals);
	sem_close(philo->dt->forks);
	sem_close(philo->dt->lock_forks);
	sem_close(philo->dt->print_output);
	sem_close(philo->dt->lock_state);
	sem_close(philo->dt->lock_dead);
	sem_close(philo->dt->lock_last);
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("print");
	sem_unlink("meals");
	sem_unlink("state");
	sem_unlink("dead");
	sem_unlink("last");
	free(philo->dt);
	exit(1);
}

void	eat_sleep(t_philo *philo)
{
	sem_post(philo->dt->lock_forks);
	put_screen(philo, EAT);
	sem_wait(philo->dt->lock_state);
	philo->state = EAT;
	sem_post(philo->dt->lock_state);
	sem_wait(philo->dt->lock_meals);
	philo->meals_count++;
	sem_post(philo->dt->lock_meals);
	sem_wait(philo->dt->lock_last);
	philo->last_eat = get_time_now();
	sem_post(philo->dt->lock_last);
	usleep(philo->dt->eat_time * 1000);
	sem_post(philo->dt->forks);
	sem_post(philo->dt->forks);
	put_screen(philo, SLEEP);
	sem_wait(philo->dt->lock_state);
	philo->state = SLEEP;
	sem_post(philo->dt->lock_state);
	usleep(philo->dt->sleep_time * 1000);
	put_screen(philo, THINK);
}
