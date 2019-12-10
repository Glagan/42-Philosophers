/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 18:31:46 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 19:52:35 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void
	copy_timestamp(char *buffer, long long timestamp)
{
	int	i;

	i = 12;
	while (timestamp > 0)
	{
		buffer[i--] = (timestamp % 10) + '0';
		timestamp /= 10;
	}
}

static void
	copy_position(char *buffer, int pos_digits, long position)
{
	int	i;

	i = 13 + pos_digits;
	while (position > 0)
	{
		buffer[i--] = (position % 10) + '0';
		position /= 10;
	}
}

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
	display_message(t_state *state, int type,
						long long timestamp, long position)
{
	static int	last = -1;
	static int	length = 0;
	static int	done = 0;
	char		*message;
	int			j;

	pthread_mutex_lock(&state->write_m);
	if (!done)
	{
		copy_timestamp(state->buffer, timestamp);
		copy_position(state->buffer, state->pos_digits, position);
		if (type != last)
		{
			message = get_message(type);
			length = 15 + state->pos_digits;
			j = 0;
			while (message[j])
				state->buffer[length++] = message[j++];
		}
		if (type == TYPE_DIED)
			done = 1;
		write(1, state->buffer, length);
	}
	pthread_mutex_unlock(&state->write_m);
}
