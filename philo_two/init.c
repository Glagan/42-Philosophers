/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 19:26:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/11 20:03:18 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static sem_t
	*open_semaphore(char const *name, int value)
{
	return (sem_open(name, O_CREAT | O_EXCL, 0644, value));
}

static int
	init_mutexes(t_state *state)
{
	sem_unlink(SEMAPHORE_FORK);
	sem_unlink(SEMAPHORE_DEAD);
	sem_unlink(SEMAPHORE_WRITE);
	if ((state->forks = open_semaphore(SEMAPHORE_FORK, state->amount / 2)) < 0
		|| (state->dead_m = open_semaphore(SEMAPHORE_DEAD, 1)) < 0
		|| (state->write_m = open_semaphore(SEMAPHORE_WRITE, 1)) < 0)
		return (1);
	if (!(state->threads =
		(pthread_t*)malloc(sizeof(*(state->threads)) * state->amount)))
		return (1);
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
	state->time_to_die = ft_atoi(argv[2]) * 1000;
	state->time_to_eat = ft_atoi(argv[3]) * 1000;
	state->time_to_sleep = ft_atoi(argv[4]) * 1000;
	state->threads = NULL;
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
