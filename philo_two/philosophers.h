/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:07 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/12 17:13:07 by ncolomer         ###   ########.fr       */
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
# define SEMAPHORE_READ		"pSemaphoreRead"

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4

# define IN_MS		0
# define IN_US		1

typedef struct	s_state
{
	long			dead;
	int				amount;
	int				available_forks;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				must_eat;
	char			*buffer;
	int				pos_digits;
	pthread_t		*threads;
	sem_t			*forks;
	sem_t			*dead_m;
	sem_t			*write_m;
	sem_t			*fork_reading;
}				t_state;

int				ft_strlen(char const *str);

int				exit_error(char const *str);

int				ft_atoi(char const *str);

int				clear_state(t_state *state);

uint64_t		get_time(void);

int				init_params(t_state *state, int argc, char const **argv);

int				should_end(t_state *state, uint64_t limit, long position);

uint64_t		eat(t_state *state, uint64_t limit, long position);

long			kill_philosopher(t_state *state, long position);

int				is_one_dead(t_state *state);

void			display_message(t_state *state, int type,
									uint64_t timestamp, long position);

#endif
