/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:07 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/10 15:35:57 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct	s_state
{
	long			dead;
	int				amount;
	long long		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	int				must_eat;
	long long		sleep_step;

	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*forks_read;
	int				*forks_used;
	pthread_mutex_t	dead_m;
}				t_state;

int			ft_strlen(char const *str);

int			exit_error(char const *str);

int			ft_atoi(char const *str);

int			clear_state(t_state *state);

long long	get_time(void);

#endif
