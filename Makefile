# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/02/18 16:00:32 by yel-touk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g -O0 -fsanitize=address

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c tokenize.c cd_pwd_builtins.c unset.c echo.c exit.c label_tokens.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

$(NAME): $(OBJS)
		@cd libft && make
		# for mac
		$(CC) -lreadline $(OBJS) $(CFLAGS) ./libft/libft.a -o $(NAME)
		# for linux
			#$(CC)  $(OBJS) $(CFLAGS) -L/usr/local/lib -I/usr/local/include -lreadline ./libft/libft.a -o $(NAME)

clean:
	@cd libft && make clean
	rm -rf $(OBJS)

fclean: clean
	@cd libft && make fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

