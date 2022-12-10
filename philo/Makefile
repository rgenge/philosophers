# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acosta-a <acosta-a@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 22:45:46 by acosta-a          #+#    #+#              #
#    Updated: 2022/12/08 20:57:38 by acosta-a         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = init_put.c utils.c philo.c philo_aux.c

FLAGS = -g -Werror -Wextra -Wall -pthread

OBJS = ${SRCS:.c=.o}

$(NAME) : $(OBJS)
	gcc $(FLAGS) $(SRCS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all make clean

.PHONY: clean fclean re
