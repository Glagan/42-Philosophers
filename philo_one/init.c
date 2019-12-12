/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 19:26:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 22:47:10 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdio.h>

static int
	init_mutexes(t_state *state)
{
	int	i;

	pthread_mutex_init(&state->write_m, NULL);
	pthread_mutex_init(&state->is_over_m, NULL);
	pthread_mutex_init(&state->somebody_dead_m, NULL);
	pthread_mutex_lock(&state->somebody_dead_m);
	if (!(state->forks_m =
		(pthread_mutex_t*)malloc(sizeof(*(state->forks_m)) * state->amount)))
		return (1);
	i = 0;
	while (i < state->amount)
		pthread_mutex_init(&state->forks_m[i++], NULL);
	return (0);
}

static void
	init_philos(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->amount)
	{
		state->philos[i].is_eating = 0;
		state->philos[i].position = i;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i + 1) % state->amount;
		state->philos[i].state = state;
		pthread_mutex_init(&state->philos[i].mutex, NULL);
		i++;
	}
}

int
	init(t_state *state, int argc, char const **argv)
{
	state->amount = ft_atoi(argv[1]);
	state->time_to_die = ft_atoi(argv[2]);
	state->time_to_eat = ft_atoi(argv[3]);
	state->time_to_sleep = ft_atoi(argv[4]);
	state->forks_m = NULL;
	state->philos = NULL;
	state->over = 0;
	if (argc == 5)
		state->must_eat = ft_atoi(argv[5]);
	else
		state->must_eat = 0;
	if (!(state->philos =
		(t_philo*)malloc(sizeof(*(state->philos)) * state->amount)))
		return (1);
	init_philos(state);
	return (init_mutexes(state));
}
