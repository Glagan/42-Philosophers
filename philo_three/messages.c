/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:31:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 16:01:52 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static char
	*get_message(int type)
{
	if (type == TYPE_EAT)
		return (" is eating\n");
	else if (type == TYPE_SLEEP)
		return (" is sleeping\n");
	else if (type == TYPE_FORK)
		return (" has taken a fork\n");
	else if (type == TYPE_THINK)
		return (" is thinking\n");
	else if (type == TYPE_OVER)
		return ("must eat count reached\n");
	return (" died\n");
}

void
	display_message(t_philo *philo, int type)
{
	sem_wait(philo->state->write_m);
	sem_wait(philo->state->dead_write_m);
	ft_putnbr_fd(get_time() - philo->state->start, 1);
	write(1, "\t", 1);
	if (type != TYPE_OVER)
		ft_putnbr_fd(philo->position + 1, 1);
	write(1, get_message(type), ft_strlen(get_message(type)));
	if (type < TYPE_DIED)
		sem_post(philo->state->dead_write_m);
	sem_post(philo->state->write_m);
}
