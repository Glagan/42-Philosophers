/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 00:43:57 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
			sem_post(philo->somebody_dead_m);
			return ((void*)0);
		}
		sem_post(philo->mutex);
		usleep(1000);
	}
	return ((void*)0);
}

static int
	routine(t_philo *philo_v)
{
	t_philo		*philo;
	pthread_t	tid;

	philo = (t_philo*)philo_v;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->state->time_to_die;
	if (pthread_create(&tid, NULL, &monitor, philo) != 0)
		return (1);
	pthread_detach(tid);
	while (1)
	{
		take_forks(philo);
		eat(philo);
		clean_forks(philo);
		display_message(philo, TYPE_THINK);
	}
	return (0);
}

static int
	start_process(t_state *state)
{
	int			i;
	void		*philo;

	state->start = get_time();
	i = 0;
	while (i < state->amount)
	{
		philo = (void*)(&state->philos[i]);
		state->philos[i].pid = fork();
		if (state->philos[i].pid < 0)
			return (1);
		else if (state->philos[i].pid == 0)
		{
			init_child_semaphores(&state->philos[i]);
			routine(&state->philos[i]);
			clear_child_semaphores(&state->philos[i]);
			exit(0);
		}
		usleep(100);
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
	if (start_process(&state))
		return (clear_state(&state) && exit_error("error: fatal\n"));
	sem_wait(state.somebody_dead_m);
	i = 0;
	while (i < state.amount)
	{
		kill(state.philos[i].pid, SIGHUP);
		clear_philo(&state.philos[i]);
		i++;
	}
	clear_state(&state);
	return (0);
}
