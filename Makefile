# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/02/08 13:22:29 by yel-touk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g -O0 -I/usr/local/Cellar/readline/8.1/include

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

$(NAME): $(OBJS)
		@cd libft && make
		$(CC) -L /usr/local/Cellar/readline/8.1/lib -lreadline $(OBJS) $(CFLAGS) ./libft/libft.a -o $(NAME)

clean:
	@cd libft && make clean
	rm -rf $(OBJS)

fclean: clean
	@cd libft && make fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
