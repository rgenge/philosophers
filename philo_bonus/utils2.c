/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:24 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/03 03:22:56 by acosta-a         ###   ########.fr       */
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
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("print");
	free(philo->dt);
	free(philo);
}

void	exit_free_close_2(t_philo *philo)
{
	sem_close(philo->dt->forks);
	sem_close(philo->dt->lock_forks);
	sem_close(philo->dt->print_output);
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("print");
	free(philo->dt);
//	free(philo);
	exit(1);
}
