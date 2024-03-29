# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/30 09:55:36 by abiru             #+#    #+#              #
#    Updated: 2023/03/31 00:08:15 by abiru            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc -I/usr/local/opt/readline/include -g3

CFLAGS = -Wextra -Werror -Wall

SRCS = main.c dict_utils.c export_builtin.c list_utils.c parse.c tokenize.c cd_pwd_builtins.c unset.c echo.c exit.c label_tokens.c expand.c parse_utils.c \
		cmd_list.c pipex_utils.c executor.c get_next_line.c heredoc.c error_utils.c init_env.c free_utils.c pipes.c \
		dup_utils.c file_utils.c builtin_utils.c envp_constructor.c cmd_tools.c cmdlist_utils.c print_env.c hd_utils.c tokenize_utils.c tokenize_utils2.c \
		label_utils.c expand_utils.c expand_utils2.c redirections.c exec_utils.c

OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $?

$(NAME): $(LIBFT) $(OBJS)
# for mac
		$(CC) $(CFLAGS) -Ilibft $(LIBFT) $(OBJS) -lreadline -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -o $@
# for linux
# $(CC) $(CFLAGS) $(SRCS) -Ilibft ./libft/libft.a -lreadline  -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -o $@

leak_check:
	 valgrind --trace-children=yes --show-leak-kinds=all --leak-check=full --show-reachable=yes --track-fds=yes --error-limit=no --suppressions=./readline.supp ./minishell

$(LIBFT):
	@make -C libft

clean:
	@make clean -C libft
	rm -rf $(OBJS)

fclean: clean
	@make fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re