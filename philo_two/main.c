/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 16:02:27 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void
	*monitor_count(void *state_v)
{
	t_state *state;
	int		total;
	int		i;

	state = (t_state*)state_v;
	total = 0;
	while (total < state->must_eat_count)
	{
		i = 0;
		while (i < state->amount)
			sem_wait(state->philos[i++].eat_count_m);
		total++;
	}
	display_message(&state->philos[0], TYPE_OVER);
	i = 0;
	while (i < state->amount)
		sem_wait(state->philos[i++].mutex);
	sem_post(state->somebody_dead_m);
	i = 0;
	while (i < state->amount)
		sem_post(state->philos[i++].mutex);
	return ((void*)0);
}

static void
	*monitor(void *philo_v)
{
	t_philo		*philo;

	philo = (t_philo*)philo_v;
	while (1)
	{
		sem_wait(philo->mutex);
		if (!philo->is_eating && get_time() > philo->limit)
		{
			display_message(philo, TYPE_DIED);
			sem_post(philo->mutex);
			sem_post(philo->state->somebody_dead_m);
			return ((void*)0);
		}
		sem_post(philo->mutex);
		usleep(1000);
	}
	return ((void*)0);
}

static void
	*routine(void *philo_v)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo*)philo_v;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->state->time_to_die;
	if (pthread_create(&tid, NULL, &monitor, philo_v) != 0)
		return ((void*)1);
	pthread_detach(tid);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		clean_forks(philo);
		display_message(philo, TYPE_THINK);
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
	int			i;
	t_state	state;

	if (argc < 5 || argc > 6)
		return (exit_error("error: bad arguments\n"));
	if (init(&state, argc, argv))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	if (start_threads(&state))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	sem_wait(state.somebody_dead_m);
	i = 0;
	while (i < state.amount)
	{
		sem_wait(state.philos[i].mutex);
		clear_philo(&state.philos[i++]);
	}
	clear_state(&state);
	return (0);
}
