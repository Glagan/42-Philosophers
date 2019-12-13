/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:02:59 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 18:29:07 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	clear_state(t_state *state)
{
	if (state->philos)
		free(state->philos);
	sem_unlink(SEMAPHORE_FORK);
	sem_unlink(SEMAPHORE_WRITE);
	sem_unlink(SEMAPHORE_DEAD);
	return (1);
}

int
	exit_error(char const *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (1);
}
