/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 19:52:32 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_state	g_state;

static long long
	loop_alive(long long last_eat, long position)
{
	long long	limit;

	limit = last_eat + g_state.time_to_die;
	if (get_time() > limit)
		return (kill_philosopher(&g_state, position));
	while (!wait_for_forks(&g_state, position))
	{
		if (is_one_dead(&g_state))
			return (0);
		else if (get_time() > limit)
			return (kill_philosopher(&g_state, position));
	}
	display_message(&g_state, TYPE_EAT, get_time(), position);
	usleep(g_state.time_to_eat);
	last_eat = get_time();
	clean_forks(&g_state, position);
	return (last_eat);
}

static void
	*philosopher_routine(void *v_pos)
{
	const long	position = (long)v_pos;
	int			alive;
	long long	last_eat;
	int			pos_rfork;

	pos_rfork = (position == g_state.amount) ? 0 : position;
	last_eat = get_time();
	alive = 1;
	while (alive && !is_one_dead(&g_state))
	{
		if ((last_eat = loop_alive(last_eat, position)) == 0)
			return ((void*)0);
		if (is_one_dead(&g_state))
			return ((void*)0);
		display_message(&g_state, TYPE_SLEEP, get_time(), position);
		usleep(g_state.time_to_sleep);
		if (is_one_dead(&g_state))
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

	if (argc < 4 || argc > 5)
		return (exit_error("error: bad arguments\n"));
	if (init_params(&g_state, argc, argv) || start_threads())
	{
		return (clear_state(&g_state)
			&& exit_error("error: fatal\n"));
	}
	i = 0;
	while (!is_one_dead(&g_state))
		(void)i;
	while (i < g_state.amount)
		pthread_detach(g_state.threads[i++]);
	display_message(&g_state, TYPE_DIED, get_time(), g_state.dead);
	clear_state(&g_state);
	return (0);
}
