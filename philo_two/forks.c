/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:40:23 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/19 20:47:12 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philosophers.h"

int
	take_forks(t_philo *philo)
{
	if (sem_wait(philo->state->forks_m))
		return (1);
	if (display_message(philo, TYPE_FORK))
		return (1);
	if (sem_wait(philo->state->forks_m))
		return (1);
	if (display_message(philo, TYPE_FORK))
		return (1);
	return (0);
}

int
	clean_forks(t_philo *philo)
{
	if (display_message(philo, TYPE_SLEEP))
		return (1);
	if (sem_post(philo->state->forks_m))
		return (1);
	if (sem_post(philo->state->forks_m))
		return (1);
	usleep(philo->state->time_to_sleep * 1000);
	return (0);
}
