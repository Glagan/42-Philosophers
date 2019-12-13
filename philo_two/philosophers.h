/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 12:40:07 by ncolomer          #+#    #+#             */
/*   Updated: 2019/12/13 18:07:56 by ncolomer         ###   ########.fr       */
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
# define SEMAPHORE_WRITE	"pSemaphoreWrite"
# define SEMAPHORE_DEAD		"pSemaphoreDead"
# define SEMAPHORE_PHILO	"pSemaphorePhilo"
# define SEMAPHORE_PHILOEAT	"pSemaphorePhiloEat"

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4
# define TYPE_OVER 	5

struct s_state;

typedef struct		s_philo
{
	int				position;
	int				is_eating;
	uint64_t		limit;
	uint64_t		last_eat;
	int				lfork;
	int				rfork;
	struct s_state	*state;
	int				eat_count;
	sem_t			*mutex;
	sem_t			*eat_count_m;
}					t_philo;

typedef struct		s_state
{
	int				amount;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				must_eat_count;
	int				cur_eat_count;

	uint64_t		start;
	int				over;

	t_philo			*philos;
	sem_t			*forks_m;
	sem_t			*write_m;
	sem_t			*somebody_dead_m;
}					t_state;

int					ft_strlen(char const *str);

int					ft_atoi(char const *str);

void				ft_putnbr_fd(uint64_t n, int fd);

int					ft_strcpy(char *dst, const char *src);

uint64_t			get_time(void);

int					clear_state(t_state *state);

int					exit_error(char const *str);

int					init(t_state *state, int argc, char const **argv);

sem_t				*ft_sem_open(char const *name, int value);

char				*make_semaphore_name(char const *base,
											char *buffer, int position);

int					take_forks(t_philo *philo);

int					clean_forks(t_philo *philo);

int					eat(t_philo *philo);

int					display_message(t_philo *philo, int type);

#endif
