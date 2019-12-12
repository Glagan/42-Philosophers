/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 19:26:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 00:51:08 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int
	init_semaphores(t_state *state)
{
	sem_unlink(SEMAPHORE_FORK);
	sem_unlink(SEMAPHORE_WRITE);
	sem_unlink(SEMAPHORE_OVER);
	sem_unlink(SEMAPHORE_DEAD);
	sem_unlink(SEMAPHORE_DEADW);
	if ((state->forks_m = ft_sem_open(SEMAPHORE_FORK, state->amount)) < 0
		|| (state->write_m = ft_sem_open(SEMAPHORE_WRITE, 1)) < 0
		|| (state->is_over_m = ft_sem_open(SEMAPHORE_OVER, 1)) < 0
		|| (state->somebody_dead_m = ft_sem_open(SEMAPHORE_DEAD, 0)) < 0
		|| (state->dead_write_m = ft_sem_open(SEMAPHORE_DEADW, 1)) < 0)
		return (1);
	return (0);
}

static int
	init_philos(t_state *state)
{
	int		i;
	char	semaphore[250];

	i = 0;
	while (i < state->amount)
	{
		make_semaphore_name((char*)semaphore, i);
		sem_unlink(semaphore);
		state->philos[i].is_eating = 0;
		state->philos[i].position = i;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i + 1) % state->amount;
		state->philos[i].state = state;
		if ((state->philos[i].mutex = ft_sem_open(semaphore, 1)) < 0)
			return (1);
		i++;
	}
	return (0);
}

int
	init(t_state *state, int argc, char const **argv)
{
	state->amount = ft_atoi(argv[1]);
	state->time_to_die = ft_atoi(argv[2]);
	state->time_to_eat = ft_atoi(argv[3]);
	state->time_to_sleep = ft_atoi(argv[4]);
	state->forks_m = NULL;
	state->philos = NULL;
	state->over = 0;
	if (argc == 5)
		state->must_eat = ft_atoi(argv[5]);
	else
		state->must_eat = 0;
	if (!(state->philos =
		(t_philo*)malloc(sizeof(*(state->philos)) * state->amount)))
		return (1);
	if (init_philos(state))
		return (1);
	return (init_semaphores(state));
}
