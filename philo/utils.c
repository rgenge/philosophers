/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 22:49:24 by acosta-a          #+#    #+#             */
/*   Updated: 2022/11/25 01:16:35 by acosta-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

void	ft_putstr_fd(char *s, int fd)
{
	int		i;
	char	c;

	i = 0;
	while (s[i])
	{
		c = s[i];
		write(fd, &c, 1);
		i++;
	}
}

static int	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ');
}

int	ft_atoi(const char *s)
{
	int	index;
	int	isneg;
	int	result;

	index = 0;
	isneg = 1;
	result = 0;
	if (!*s)
		return (0);
	while (ft_isspace(s[index]) == 1)
		index++;
	if (s[index] == '-' || s[index] == '+')
	{
		if (s[index] == '-')
			isneg = isneg * -1;
		index++;
	}
	while (s[index] >= '0' && s[index] <= '9')
	{
		result = (result * 10) + s[index] - '0';
		index++;
	}
	return (result * isneg);
}

long long int	get_time_now(void)
{
	long long int			ms;
	static long long int	start = 0;
	static struct timeval	time;

	gettimeofday(&time, NULL);
	ms = time.tv_sec * 1000;
	ms += time.tv_usec / 1000;
	if (!start)
		start = ms;
	return (ms - start);
}
