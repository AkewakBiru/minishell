# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/02/12 15:04:05 by abiru            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g -O0

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c tokenize.c cd_pwd_builtins.c unset.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

$(NAME): $(OBJS)
		@cd libft && make
		$(CC) -lreadline $(OBJS) $(CFLAGS) ./libft/libft.a -o $(NAME)

clean:
	@cd libft && make clean
	rm -rf $(OBJS)

fclean: clean
	@cd libft && make fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
