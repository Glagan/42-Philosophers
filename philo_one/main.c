/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 23:03:56 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int
	is_over(t_state *state)
{
	int	over;

	pthread_mutex_lock(&state->is_over_m);
	over = state->over;
	pthread_mutex_unlock(&state->is_over_m);
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
		pthread_mutex_lock(&philo->mutex);
		if (!philo->is_eating && get_time() > philo->limit)
		{
			display_message(philo, TYPE_DIED);
			pthread_mutex_unlock(&philo->mutex);
			pthread_mutex_unlock(&philo->state->somebody_dead_m);
			return ((void*)0);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(1000);
	}
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
	t_state	state;

	if (argc < 4 || argc > 5)
		return (exit_error("error: bad arguments\n"));
	if (init(&state, argc, argv))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	if (start_threads(&state))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	pthread_mutex_lock(&state.somebody_dead_m);
	pthread_mutex_unlock(&state.somebody_dead_m);
	clear_state(&state);
	return (0);
}
