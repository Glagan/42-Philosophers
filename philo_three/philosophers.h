/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:07 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 00:52:50 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

# define SEMAPHORE_FORK		"pSemaphoreFork"
# define SEMAPHORE_WRITE	"pSemaphoreWrite"
# define SEMAPHORE_OVER		"pSemaphoreOver"
# define SEMAPHORE_DEAD		"pSemaphoreDead"
# define SEMAPHORE_DEADW	"pSemaphoreDeadWrite"

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4

struct s_state;

typedef struct		s_philo
{
	pid_t			pid;
	int				position;
	int				is_eating;
	uint64_t		limit;
	uint64_t		last_eat;
	int				lfork;
	int				rfork;
	struct s_state	*state;
	sem_t			*mutex;
}					t_philo;

typedef struct		s_state
{
	int				amount;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				must_eat;

	uint64_t		start;
	int				over;

	t_philo			*philos;
	sem_t			*forks_m;
	sem_t			*write_m;
	sem_t			*is_over_m;
	sem_t			*somebody_dead_m;
	sem_t			*dead_write_m;
}					t_state;

int					ft_strlen(char const *str);

int					ft_atoi(char const *str);

void				ft_putnbr_fd(uint64_t n, int fd);

int					ft_strcpy(char *dst, const char *src);

uint64_t			get_time(void);

int					clear_philo(t_philo *philo);

int					clear_state(t_state *state);

int					exit_error(char const *str);

int					init(t_state *state, int argc, char const **argv);

sem_t				*ft_sem_open(char const *name, int value);

char				*make_semaphore_name(char *buffer, int position);

void				take_forks(t_philo *philo);

void				clean_forks(t_philo *philo);

void				eat(t_philo *philo);

void				display_message(t_philo *philo, int type);

#endif
