/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:21:51 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 19:12:08 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int
	wait_for_forks(t_state *state, long position)
{
	int rfork;
	int	can_eat;

	rfork = (position == state->amount) ? 0 : position;
	pthread_mutex_lock(&state->fork_reading);
	can_eat = 0;
	if (!state->forks[position - 1] && !state->forks[rfork])
	{
		can_eat = 1;
		state->forks[position - 1] = 1;
		state->forks[rfork] = 1;
		display_message(state, TYPE_FORK, get_time(), position);
	}
	pthread_mutex_unlock(&state->fork_reading);
	return (can_eat);
}

void
	clean_forks(t_state *state, long position)
{
	int rfork;

	rfork = (position == state->amount) ? 0 : position;
	pthread_mutex_lock(&state->fork_reading);
	state->forks[position - 1] = 0;
	state->forks[rfork] = 0;
	pthread_mutex_unlock(&state->fork_reading);
}
