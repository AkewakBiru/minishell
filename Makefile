# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/02/21 11:45:37 by yel-touk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g -O0 -fsanitize=address

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c tokenize.c cd_pwd_builtins.c unset.c echo.c exit.c label_tokens.c expand.c

LIBFT = ./libft/libft.a

all: $(NAME)

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $?

$(NAME): $(LIBFT) $(SRCS)
		# for mac
		$(CC) $(CFLAGS) -Ilibft $(LIBFT) $(SRCS) -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -o $@
		# for linux
		$(CC) $(CFLAGS) -Ilibft $(LIBFT) $(SRCS) -lreadline -L/usr/local/lib -I/usr/local/include -o $@

$(LIBFT):
	@make -C libft

clean:
	@make clean -C libft
	# rm -rf $(OBJS)

fclean: clean
	@make fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

