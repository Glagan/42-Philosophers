/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 15:30:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 15:55:19 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "utils.h"

t_state	g_state;

int
	should_die(long position, long long last_eat)
{
	pthread_mutex_lock(&g_state.dead_m);
	if (g_state.dead)
	{
		pthread_mutex_unlock(&g_state.dead_m);
		return (1);
	}
	if (last_eat > 0 && get_time() - last_eat > g_state.time_to_die)
	{
		g_state.dead = position;
		pthread_mutex_unlock(&g_state.dead_m);
		return (1);
	}
	pthread_mutex_unlock(&g_state.dead_m);
	return (0);
}

int
	wait_for_forks(long position)
{
	int rfork;
	int	can_eat;

	rfork = (position == g_state.amount) ? 0 : position;
	pthread_mutex_lock(&g_state.forks_read[position - 1]);
	pthread_mutex_lock(&g_state.forks_read[rfork]);
	can_eat = 0;
	if (!g_state.forks_used[position - 1] && !g_state.forks_used[rfork])
	{
		can_eat = 1;
		g_state.forks_used[position - 1] = 1;
		g_state.forks_used[rfork] = 1;
		pthread_mutex_lock(&g_state.forks[position - 1]);
		pthread_mutex_lock(&g_state.forks[rfork]);
		printf("%lld %ld has taken a fork\n", get_time(), position);
	}
	pthread_mutex_unlock(&g_state.forks_read[position - 1]);
	pthread_mutex_unlock(&g_state.forks_read[rfork]);
	return (can_eat);
}

void
	clean_forks(long position)
{
	int rfork;

	rfork = (position == g_state.amount) ? 0 : position;
	pthread_mutex_lock(&g_state.forks_read[position - 1]);
	pthread_mutex_lock(&g_state.forks_read[rfork]);
	g_state.forks_used[position - 1] = 0;
	g_state.forks_used[rfork] = 0;
	pthread_mutex_unlock(&g_state.forks[position - 1]);
	pthread_mutex_unlock(&g_state.forks[rfork]);
	pthread_mutex_unlock(&g_state.forks_read[position - 1]);
	pthread_mutex_unlock(&g_state.forks_read[rfork]);
}

void
	*philosopher_routine(void *v_pos)
{
	const long	position = (long)v_pos;
	int			alive;
	long long	last_eat;
	int			pos_rfork;
	useconds_t	i;

	pos_rfork = (position == g_state.amount) ? 0 : position;
	last_eat = get_time();
	printf("%lld %ld is alive\n", get_time(), position);
	alive = 1;
	while (alive && !g_state.dead)
	{
		if (should_die(position, last_eat))
			break ;
		while (!wait_for_forks(position))
			if (should_die(position, last_eat))
				pthread_exit((void*) 0);
		printf("%lld %ld is eating\n", get_time(), position);
		i = 0;
		while (i < g_state.time_to_eat)
		{
			usleep(1);
			if (should_die(position, 0))
			{
				clean_forks(position);
				pthread_exit((void*) 0);
			}
			i++;
		}
		last_eat = get_time();
		clean_forks(position);
		if (should_die(position, last_eat))
			break ;
		printf("%lld %ld is sleeping\n", get_time(), position);
		i = 0;
		while (i < g_state.time_to_sleep)
		{
			usleep(1);
			if (should_die(position, last_eat))
				pthread_exit((void*) 0);
			i++;
		}
		printf("%lld %ld is thinking\n", get_time(), position);
	}
	pthread_exit((void*) 0);
}

int
	init_params(int argc, char const **argv)
{
	int	i;

	g_state.amount = ft_atoi(argv[1]);
	g_state.time_to_die = ft_atoi(argv[2]);
	g_state.time_to_eat = ft_atoi(argv[3]);
	g_state.time_to_sleep = ft_atoi(argv[4]);
	g_state.threads = NULL;
	g_state.forks = NULL;
	g_state.dead = 0;
	pthread_mutex_init(&g_state.dead_m, NULL);
	if (argc == 5)
		g_state.must_eat = ft_atoi(argv[5]);
	if (!(g_state.threads =
		(pthread_t*)malloc(sizeof(*(g_state.threads)) * g_state.amount)))
		return (1);
	if (!(g_state.forks =
		(pthread_mutex_t*)malloc(sizeof(*(g_state.forks)) * g_state.amount)))
		return (1);
	if (!(g_state.forks_read =
		(pthread_mutex_t*)malloc(sizeof(*(g_state.forks)) * g_state.amount)))
		return (1);
	if (!(g_state.forks_used =
		(int*)malloc(sizeof(*(g_state.forks_used)) * g_state.amount)))
		return (1);
	i = 0;
	while (i < g_state.amount)
	{
		pthread_mutex_init(&g_state.forks[i], NULL);
		pthread_mutex_init(&g_state.forks_read[i], NULL);
		g_state.forks_used[i++] = 0;
	}
	return (0);
}

int
	start_threads(void)
{
	long			i;
	pthread_attr_t	attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	i = 0;
	while (i < g_state.amount)
	{
		pthread_create(&g_state.threads[i], &attr, &philosopher_routine, (void*)(i + 1));
		i++;
	}
	printf("%lld all threads created\n", get_time());
	pthread_attr_destroy(&attr);
	return (0);
}

/*
** 1: Nombre de philosophes et de fourchettes
** 2: Si un philosophe ne mange pas avant time_to_die (debut ou dernier repas), il meurt
** 3: Temps que mets un philosophe pour manger, il utilise 2 fourchettes
** 4: Temps que mets le philosophe pour dormir
** 5: Nombre de fois que *tout* les philosophes doivent manger avant que la simulation s'arrete
*/
int main(int argc, char const **argv)
{
	int			i;
	void		*status;

	if (argc < 4 || argc > 5)
		return (exit_error("error: bad arguments\n"));
	if (init_params(argc, argv))
		return (clear_state(&g_state) && exit_error("error: fatal\n"));
	printf("%lld simulation start\n", get_time());
	if (start_threads())
		return (clear_state(&g_state) && exit_error("error: failed to start threads\n"));
	i = 0;
	while (i < g_state.amount)
	{
		pthread_join(g_state.threads[i++], &status);
		if (i == g_state.dead)
			printf("%lld %ld died\n", get_time(), g_state.dead);
	}
	clear_state(&g_state);
	pthread_exit(NULL);
}
