/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 19:26:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 17:18:46 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int
	init_mutexes(t_state *state)
{
	int	i;

	pthread_mutex_init(&state->dead_m, NULL);
	pthread_mutex_init(&state->fork_reading, NULL);
	pthread_mutex_init(&state->write_m, NULL);
	if (!(state->threads =
		(pthread_t*)malloc(sizeof(*(state->threads)) * state->amount)))
		return (1);
	if (!(state->forks =
		(int*)malloc(sizeof(*(state->forks)) * state->amount)))
		return (1);
	i = 0;
	while (i < state->amount)
		state->forks[i++] = 0;
	return (0);
}

static int
	init_buffer(t_state *state)
{
	state->buffer = NULL;
	if (!(state->buffer =
		(char*)malloc(sizeof(*state->buffer) * (33 + state->pos_digits))))
	{
		return (1);
	}
	state->buffer[13] = ' ';
	state->buffer[14 + state->pos_digits] = ' ';
	return (0);
}

int
	init_params(t_state *state, int argc, char const **argv)
{
	int	tmp;

	state->amount = ft_atoi(argv[1]);
	state->time_to_die = ft_atoi(argv[2]);
	state->time_to_eat = ft_atoi(argv[3]);
	state->time_to_sleep = ft_atoi(argv[4]);
	state->threads = NULL;
	state->forks = NULL;
	state->dead = 0;
	state->pos_digits = 1;
	tmp = state->amount;
	while (tmp > 10)
	{
		state->pos_digits++;
		tmp /= 10;
	}
	if (init_buffer(state))
		return (1);
	if (argc == 5)
		state->must_eat = ft_atoi(argv[5]);
	return (init_mutexes(state));
}
