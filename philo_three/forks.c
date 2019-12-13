/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:40:23 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 15:36:13 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void
	take_forks(t_philo *philo)
{
	sem_wait(philo->state->forks_m);
	display_message(philo, TYPE_FORK);
	sem_wait(philo->state->forks_m);
	display_message(philo, TYPE_FORK);
}

void
	clean_forks(t_philo *philo)
{
	display_message(philo, TYPE_SLEEP);
	sem_post(philo->state->forks_m);
	sem_post(philo->state->forks_m);
	usleep(philo->state->time_to_sleep * 1000);
}
