# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/02/28 23:12:52 by abiru            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -g 

#-O0 -fsanitize=address

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c tokenize.c cd_pwd_builtins.c unset.c echo.c exit.c label_tokens.c expand.c parse_utils.c \
		cmd_list.c pipex_utils.c executor.c get_next_line.c heredoc.c error_utils.c redir_in_out.c

LIBFT = ./libft/libft.a

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

$(NAME): $(LIBFT) $(SRCS)
# for mac
		$(CC) $(CFLAGS) -Ilibft $(LIBFT) $(SRCS) -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -o $@
# for linux
# $(CC) $(CFLAGS) $(SRCS) -Ilibft ./libft/libft.a -lreadline  -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -o $@

leak_check:
	 valgrind --trace-children=yes --show-leak-kinds=all --leak-check=full --show-reachable=yes --track-fds=yes --error-limit=no --suppressions=./readline.supp ./minishell

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

