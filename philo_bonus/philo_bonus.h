/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 23:10:45 by acosta-a          #+#    #+#             */
/*   Updated: 2022/12/03 03:08:58 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# define START 0
# define ERROR 1
# define STDERR	2
# define EAT 2
# define SLEEP 3
# define THINK 1
# define DEAD 4
# define LEFT_FORK 5
# define RIGHT_FORK 6

typedef struct s_dt
{
	int				i;
	int				dead;
	int				num_philo;
	long long		start_time;
	long long		die_time;
	long long		eat_time;
	long long		sleep_time;
	int				n_must_eat;
	int				all_eat;
	long long		start;
	int				status;
	sem_t			*forks;
	sem_t			*print_output;
	sem_t			*lock_forks;
	int				free_flag;
}	t_dt;

typedef struct s_philo
{
	int					id;
	pid_t				pid;
	long long			curr_time;
	long long			last_eat;
	int					meals_count;
	t_dt				*dt;
	pthread_t			t;
	int					state;
}	t_philo;
/*init_put*/
void				exit_free(t_dt *dt, t_philo *philo, int flag);
int					init(t_dt *dt, t_philo **philo, int argc, char *argv[]);
void				put_screen(t_philo *philo, int doing);
/*philo*/
void				create_philos(t_philo *philo, t_dt *dt);
void				init_philos(t_dt *dt, t_philo *philo);
void				*check_death(void *arg);
/*utils*/
void				ft_putstr_fd(char *s, int fd);
int					ft_atoi(const char *s);
long long int		get_time_now(void);
void				exit_free_close(t_philo *philo);
void				kill_process(t_philo *philo);
void				exit_free_close_2(t_philo *philo);
#endif
