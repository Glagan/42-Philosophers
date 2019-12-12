/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 23:50:38 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int
	is_over(t_state *state)
{
	int	over;

	sem_wait(state->is_over_m);
	over = state->over;
	sem_post(state->is_over_m);
	return (over);
}

static void
	*monitor(void *philo_v)
{
	t_philo		*philo;

	philo = (t_philo*)philo_v;
	while (1)
	{
		if (is_over(philo->state))
			return ((void*)0);
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

	philo = (t_philo*)philo_v;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->state->time_to_die;
	while (1)
	{
		take_forks(philo);
		eat(philo);
		clean_forks(philo);
		if (is_over(philo->state))
			return ((void*)0);
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

	state->start = get_time();
	i = 0;
	while (i < state->amount)
	{
		philo = (void*)(&state->philos[i]);
		if (pthread_create(&tid, NULL, &routine, philo) != 0)
			return (1);
		usleep(100);
		pthread_detach(tid);
		if (pthread_create(&tid, NULL, &monitor, philo) != 0)
			return (1);
		pthread_detach(tid);
		i++;
	}
	return (0);
}

int
	main(int argc, char const **argv)
{
	int		i;
	t_state	state;

	if (argc < 4 || argc > 5)
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
		clear_philo(&state.philos[i]);
		i++;
	}
	clear_state(&state);
	return (0);
}
