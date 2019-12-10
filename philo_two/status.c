/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:22:49 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 19:52:39 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	kill_philosopher(t_state *state, long position)
{
	pthread_mutex_lock(&state->dead_m);
	if (!state->dead)
		state->dead = position;
	pthread_mutex_unlock(&state->dead_m);
	return (0);
}

int
	is_one_dead(t_state *state)
{
	int	is_there;

	is_there = 0;
	pthread_mutex_lock(&state->dead_m);
	is_there = !!state->dead;
	pthread_mutex_unlock(&state->dead_m);
	return (is_there);
}
