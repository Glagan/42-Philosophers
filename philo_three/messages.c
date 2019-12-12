/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:31:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 00:35:17 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static char
	*get_message(int type)
{
	if (type == TYPE_EAT)
		return ("is eating\n");
	else if (type == TYPE_SLEEP)
		return ("is sleeping\n");
	else if (type == TYPE_FORK)
		return ("has taken a fork\n");
	else if (type == TYPE_THINK)
		return ("is thinking\n");
	return ("died\n");
}

void
	display_message(t_philo *philo, int type)
{
	sem_wait(philo->write_m);
	sem_wait(philo->dead_write_m);
	ft_putnbr_fd(get_time() - philo->state->start, 1);
	write(1, "\t", 1);
	ft_putnbr_fd(philo->position + 1, 1);
	write(1, " ", 1);
	write(1, get_message(type), ft_strlen(get_message(type)));
	if (type != TYPE_DIED)
		sem_post(philo->dead_write_m);
	sem_post(philo->write_m);
}