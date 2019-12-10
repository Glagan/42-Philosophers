/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:45 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 19:19:17 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int
	ft_strlen(char const *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int
	exit_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}

int
	ft_atoi(char const *str)
{
	int	i;
	int	start;
	int	is_neg;
	int	res;

	if (!str)
		return (0);
	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' ||
			str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	is_neg = (str[i] == '-') ? -1 : 1;
	if (is_neg == -1 || str[i] == '+')
		i++;
	start = i;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
		res = (res * 10) + (str[i++] - '0');
	return (res * is_neg);
}

int
	clear_state(t_state *state)
{
	if (state->threads)
		free(state->threads);
	if (state->forks)
		free(state->forks);
	if (state->buffer)
		free(state->buffer);
	pthread_mutex_destroy(&state->dead_m);
	pthread_mutex_destroy(&state->fork_reading);
	pthread_mutex_destroy(&state->write_m);
	return (1);
}

long long
	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}
