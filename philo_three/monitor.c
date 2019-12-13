/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 16:08:16 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 16:08:39 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void
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
	sem_post(state->somebody_dead_m);
	return ((void*)0);
}

void
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

int
	start_monitor_thread(t_state *state)
{
	pthread_t	tid;

	if (state->must_eat_count > 0)
	{
		if (pthread_create(&tid, NULL, &monitor_count, (void*)state) != 0)
			return (1);
		pthread_detach(tid);
	}
	return (0);
}
