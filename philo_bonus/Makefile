# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/16 11:35:17 by kchaouki          #+#    #+#              #
#    Updated: 2023/04/10 23:18:10 by kchaouki         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

SRCS = philo_bonus.c init_bonus.c utils_bonus_1.c utils_bonus_2.c

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

CCe = cc $(FLAGS)

all: $(LIBFT) $(NAME)

%.o : %.c
	$(CCe) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) philo_bonus.h
	$(CCe) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re