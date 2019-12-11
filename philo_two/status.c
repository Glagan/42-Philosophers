/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:22:49 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/11 17:52:59 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long
	kill_philosopher(t_state *state, long position)
{
	sem_wait(state->dead_m);
	if (!state->dead)
		state->dead = position;
	sem_post(state->dead_m);
	return (0);
}

int
	is_one_dead(t_state *state)
{
	int	is_there;

	is_there = 0;
	sem_wait(state->dead_m);
	is_there = !!state->dead;
	sem_post(state->dead_m);
	return (is_there);
}
