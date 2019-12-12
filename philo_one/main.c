/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 17:41:01 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_state	g_state;

int
	should_end(uint64_t limit, long position)
{
	if (is_one_dead(&g_state))
		return (1);
	if (get_time() > limit)
		return (!kill_philosopher(&g_state, position));
	return (0);
}

static uint64_t
	try_to_eat(uint64_t last_eat, long position)
{
	uint64_t	limit;

	limit = last_eat + g_state.time_to_die;
	while (!wait_for_forks(&g_state, position))
	{
		if (should_end(limit, position))
			return (0);
		usleep(400);
	}
	if (should_end(limit, position))
		return (0);
	display_message(&g_state, TYPE_EAT, get_time(), position);
	last_eat = get_time();
	usleep(g_state.time_to_eat * 1000);
	clean_forks(&g_state, position);
	return (last_eat);
}

static void
	*philosopher_routine(void *v_pos)
{
	const long	position = (long)v_pos;
	uint64_t	last_eat;
	uint64_t	limit;

	last_eat = get_time();
	while (!is_one_dead(&g_state))
	{
		if ((last_eat = try_to_eat(last_eat, position)) == 0)
			return ((void*)0);
		limit = last_eat + g_state.time_to_die;
		if (should_end(limit, position))
			return (0);
		display_message(&g_state, TYPE_SLEEP, get_time(), position);
		usleep(g_state.time_to_sleep * 1000);
		if (should_end(limit, position))
			return (0);
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
	while (!is_one_dead(&g_state))
		usleep(1000);
	display_message(&g_state, TYPE_DIED, get_time(), g_state.dead);
	i = 0;
	while (i < g_state.amount)
		pthread_join(g_state.threads[i++], &status);
	clear_state(&g_state);
	return (0);
}
