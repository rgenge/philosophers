/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:24 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/08 21:03:14 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	exit_free_close_3(t_philo *philo)
{
	sem_post(philo->dt->lock_meals);
	sem_close(philo->dt->forks);
	sem_close(philo->dt->lock_forks);
	sem_close(philo->dt->print_output);
	sem_unlink("forks");
	sem_unlink("lock");
	sem_unlink("print");
	sem_unlink("meals");
	sem_unlink("state");
	sem_unlink("dead");
	free(philo->dt);
	exit(2);
}
