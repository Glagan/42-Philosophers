/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:08:54 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 17:48:57 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	should_end(t_state *state, uint64_t limit, long position)
{
	if (is_one_dead(state))
		return (1);
	if (get_time() > limit)
		return (!kill_philosopher(state, position));
	return (0);
}

static int
	can_eat(t_state *state, long position)
{
	int	available;

	sem_wait(state->fork_reading);
	if ((available = (state->available_forks > 0)))
	{
		display_message(state, TYPE_FORK, get_time(), position);
		state->available_forks--;
	}
	sem_post(state->fork_reading);
	return (available);
}

static void
	clean_fork(t_state *state)
{
	sem_wait(state->fork_reading);
	state->available_forks++;
	sem_post(state->fork_reading);
}

uint64_t
	eat(t_state *state, uint64_t limit, long position)
{
	uint64_t	last_eat;

	while (!can_eat(state, position))
	{
		if (should_end(state, limit, position))
			return (0);
		usleep(400);
	}
	if (should_end(state, limit, position))
		return (0);
	display_message(state, TYPE_EAT, get_time(), position);
	last_eat = get_time();
	limit = last_eat + state->time_to_die;
	if ((last_eat + state->time_to_eat) > limit)
		usleep((limit - last_eat) * 990);
	else
		usleep(state->time_to_eat * 990);
	clean_fork(state);
	return (last_eat);
}
