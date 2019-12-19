/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/19 21:44:50 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philosophers.h"

static void
	*monitor_count(void *state_v)
{
	t_state *state;
	int		i;

	state = (t_state*)state_v;
	while (state->cur_eat_count < state->must_eat_count)
	{
		i = 0;
		while (i < state->amount)
			if (sem_wait(state->philos[i++].eat_count_m))
				return ((void*)0);
		state->cur_eat_count++;
	}
	if (display_message(&state->philos[0], TYPE_OVER))
		return ((void*)0);
	if (sem_post(state->somebody_dead_m))
		return ((void*)0);
	return ((void*)0);
}

static void
	*monitor(void *philo_v)
{
	t_philo		*philo;

	philo = (t_philo*)philo_v;
	while (1)
	{
		if (sem_wait(philo->mutex) != 0)
			return ((void*)0);
		if (!philo->is_eating && get_time() > philo->limit)
		{
			if (display_message(philo, TYPE_DIED))
				return ((void*)0);
			if (sem_post(philo->mutex))
				return ((void*)0);
			if (sem_post(philo->state->somebody_dead_m))
				return ((void*)0);
			return ((void*)0);
		}
		if (sem_post(philo->mutex))
			return ((void*)0);
	}
	return ((void*)0);
}

static void
	*routine(void *philo_v)
{
	t_philo		*philo;
	pthread_t	tid;
	int			should_end;

	philo = (t_philo*)philo_v;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->state->time_to_die;
	if (pthread_create(&tid, NULL, &monitor, philo_v) != 0)
		return ((void*)1);
	pthread_detach(tid);
	while (1)
	{
		if (take_forks(philo))
			return ((void*)0);
		should_end = eat(philo);
		if (clean_forks(philo))
			return ((void*)0);
		if (should_end)
			return ((void*)0);
		if (display_message(philo, TYPE_THINK))
			return ((void*)0);
	}
	return ((void*)0);
}

static int
	start_threads(t_state *state)
{
	int			i;
	pthread_t	tid;
	void		*philo;

	if (state->must_eat_count > 0)
	{
		if (pthread_create(&tid, NULL, &monitor_count, (void*)state) != 0)
			return (1);
		pthread_detach(tid);
	}
	state->start = get_time();
	i = 0;
	while (i < state->amount)
	{
		philo = (void*)(&state->philos[i]);
		if (pthread_create(&tid, NULL, &routine, philo) != 0)
			return (1);
		pthread_detach(tid);
		usleep(100);
		i++;
	}
	return (0);
}

int
	main(int argc, char const **argv)
{
	t_state	state;

	if (argc < 5 || argc > 6)
		return (exit_error("error: bad arguments\n"));
	if (init(&state, argc, argv)
		|| start_threads(&state))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	sem_wait(state.somebody_dead_m);
	clear_state(&state);
	return (0);
}
