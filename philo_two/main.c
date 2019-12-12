/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 17:13:18 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philosophers.h"

t_state	g_state;

static void
	*philosopher_routine(void *v_pos)
{
	const long	position = (long)v_pos;
	uint64_t	last_eat;
	uint64_t	limit;
	uint64_t	curr_time;

	limit = get_time() + g_state.time_to_die;
	while (!is_one_dead(&g_state))
	{
		if ((last_eat = eat(&g_state, limit, position)) == 0)
			return ((void*)0);
		limit = last_eat + g_state.time_to_die;
		if (should_end(&g_state, limit, position))
			return ((void*)0);
		display_message(&g_state, TYPE_SLEEP, get_time(), position);
		curr_time = get_time();
		if ((curr_time + g_state.time_to_sleep) >= limit)
			usleep((limit * 990) - (curr_time * 990));
		else
			usleep(g_state.time_to_sleep * 990);
		if (should_end(&g_state, limit, position))
			return ((void*)0);
		display_message(&g_state, TYPE_THINK, get_time(), position);
	}
	return ((void*)0);
}

static int
	start_threads(void)
{
	long			i;
	pthread_attr_t	attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	i = 0;
	while (i < g_state.amount)
	{
		pthread_create(&g_state.threads[i],
			&attr, &philosopher_routine, (void*)(i + 1));
		i++;
	}
	pthread_attr_destroy(&attr);
	return (0);
}

int
	main(int argc, char const **argv)
{
	int		i;
	void	*status;

	if (argc < 4 || argc > 5)
		return (exit_error("error: bad arguments\n"));
	if (init_params(&g_state, argc, argv) || start_threads())
	{
		return (clear_state(&g_state)
			&& exit_error("error: fatal\n"));
	}
	i = 0;
	while (!is_one_dead(&g_state))
		usleep(1000);
	display_message(&g_state, TYPE_DIED, get_time(), g_state.dead);
	while (i < g_state.amount)
		pthread_join(g_state.threads[i++], &status);
	clear_state(&g_state);
	return (0);
}
