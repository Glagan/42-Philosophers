/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:02:59 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 00:51:00 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	clear_philo(t_philo *philo)
{
	char	semaphore[250];

	sem_close(philo->mutex);
	make_semaphore_name((char*)semaphore, philo->position);
	sem_unlink(semaphore);
	return (0);
}

int
	clear_state(t_state *state)
{
	if (state->philos)
		free(state->philos);
	sem_close(state->write_m);
	sem_unlink(SEMAPHORE_WRITE);
	sem_close(state->is_over_m);
	sem_unlink(SEMAPHORE_OVER);
	sem_close(state->somebody_dead_m);
	sem_unlink(SEMAPHORE_DEAD);
	sem_close(state->dead_write_m);
	sem_unlink(SEMAPHORE_DEADW);
	return (1);
}

int
	exit_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}
