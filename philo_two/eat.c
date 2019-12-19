/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 22:41:58 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/19 20:46:54 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int
	eat(t_philo *philo)
{
	if (sem_wait(philo->mutex) != 0)
		return (1);
	philo->is_eating = 1;
	philo->last_eat = get_time();
	philo->limit = philo->last_eat + philo->state->time_to_die;
	if (display_message(philo, TYPE_EAT))
		return (1);
	usleep(philo->state->time_to_eat * 1000);
	philo->eat_count++;
	philo->is_eating = 0;
	if (sem_post(philo->mutex))
		return (1);
	if (sem_post(philo->eat_count_m))
		return (1);
	return (0);
}
