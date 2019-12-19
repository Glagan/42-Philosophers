/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:02:59 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/19 20:50:17 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	clear_state(t_state *state)
{
	int		i;
	char	semaphore[255];

	sem_unlink(SEMAPHORE_FORK);
	sem_unlink(SEMAPHORE_WRITE);
	sem_unlink(SEMAPHORE_DEAD);
	sem_unlink(SEMAPHORE_DEADW);
	if (state->philos)
	{
		i = 0;
		while (i < state->amount)
		{
			make_semaphore_name(SEMAPHORE_PHILO, (char*)semaphore, i);
			sem_unlink(semaphore);
			make_semaphore_name(SEMAPHORE_PHILOEAT, (char*)semaphore, i++);
			sem_unlink(semaphore);
		}
		free(state->philos);
	}
	return (1);
}

int
	exit_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}
