/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:07 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/11 16:07:44 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>

# define SEMAPHORE_FORK		"pSemaphoreFork"
# define SEMAPHORE_DEAD		"pSemaphoreDead"
# define SEMAPHORE_WRITE	"pSemaphoreWrite"

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4

typedef struct	s_state
{
	long			dead;
	int				amount;
	long long		time_to_die;
	useconds_t		time_to_eat;
	useconds_t		time_to_sleep;
	int				must_eat;
	char			*buffer;
	int				pos_digits;
	pthread_t		*threads;
	sem_t			*forks;
	sem_t			*dead_m;
	sem_t			*write_m;
}				t_state;

int				ft_strlen(char const *str);

int				exit_error(char const *str);

int				ft_atoi(char const *str);

int				clear_state(t_state *state);

long long		get_time(void);

int				init_params(t_state *state, int argc, char const **argv);

int				wait_for_forks(t_state *state, long position);

void			clean_forks(t_state *state, long position);

int				kill_philosopher(t_state *state, long position);

int				is_one_dead(t_state *state);

void			display_message(t_state *state, int type,
									long long timestamp, long position);

#endif
