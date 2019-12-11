/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/11 20:08:11 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_state	g_state;

static int
	should_end(uint64_t limit, long position, int do_post)
{
	if (is_one_dead(&g_state))
	{
		if (do_post)
			sem_post(g_state.forks);
		return (1);
	}
	if ((get_time() / 1000) > (limit / 1000))
	{
		if (do_post)
			sem_post(g_state.forks);
		return (!kill_philosopher(&g_state, position));
	}
	return (0);
}

static uint64_t
	eat(uint64_t last_eat, uint64_t limit, long position)
{
	uint64_t	curr_time;

	sem_wait(g_state.forks);
	if (should_end(limit, position, 1))
		return (0);
	display_message(&g_state, TYPE_EAT, get_time(), position);
	last_eat = get_time();
	limit = last_eat + g_state.time_to_die;
	curr_time = get_time();
	if ((curr_time + g_state.time_to_eat) > limit)
		usleep(limit - curr_time);
	else
		usleep(g_state.time_to_eat);
	sem_post(g_state.forks);
	return (last_eat);
}

static void
	*philosopher_routine(void *v_pos)
{
	const long	position = (long)v_pos;
	uint64_t	last_eat;
	uint64_t	limit;

	limit = get_time() + g_state.time_to_die;
	while (!is_one_dead(&g_state))
	{
		if ((last_eat = eat(last_eat, limit, position)) == 0)
			return ((void*)0);
		limit = last_eat + g_state.time_to_die;
		if (should_end(limit, position, 0))
			return ((void*)0);
		display_message(&g_state, TYPE_SLEEP, get_time(), position);
		if ((get_time() + g_state.time_to_sleep) > limit)
			usleep(limit - get_time());
		else
			usleep(g_state.time_to_sleep);
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
