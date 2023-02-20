/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:58:30 by abiru             #+#    #+#             */
/*   Updated: 2023/02/20 16:42:10 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

enum e_input_type
{
	cmd,
	option,
	arg,
	pip,
	redir_in,
	redir_out,
	here_doc,
	redir_out_append,
	file,
	delimiter,
	unset,
	redirection
};

typedef struct s_token
{
	char				*token;
	enum e_input_type	type;
}	t_token;

// temp for redirection
typedef struct cmd_op
{
	int		redir_in;
	int		redir_out;
}	t_cmd_op;

typedef struct cmd_utils
{
	char	*cmd;
	char	**cmd_arg;
	char	*key;
	char	*value;
	int		flag;
	char	*pwd;
	int		EXIT_STATUS;
	int		num_cmd;
	t_cmd_op	cmd_op;
}	t_utils;

void	check_quotes(char c, int *s_quote, int *d_quote);
void	free_tokens(t_token ***tokens_p);
t_token	**parse(char *line, t_list	*lst);
int		label_tokens(t_token ***tokens);
t_token	**tokenize(char const *line);

// dictionary utils
void	free_dict(t_dict	*dict);
int		key_exists(char *s, t_list **new);
t_dict	*create_dict(char *key, char *value, int flag);
int		update_dict(char *key, char *value, t_list **head);
char	*get_val(t_list **lst, char *key);

// export and env utils
void	print_env(t_list **lst);
void	print_list(t_list **lst);
void	update_env(t_utils *cmd, t_list **env);
void	export_bltin(t_list **lst, t_utils *cmd_utils, t_list **export);
void	create_env(t_list **head, char **envp);

// cd and pwd utils
char	*get_pwd(void);
void	print_pwd(void);
void	chg_dir(t_utils *cmd_utils, t_list **lst, t_list **export);

// linked list utils
void	sort_list(t_list **head);
void	ft_list_remove_if(t_list **head, char *str, int (*cmp)());

// echo
void	echo(char **arg);

// unset
void	unset_builtin(t_utils *cmd_utils, t_list **lst, t_list **export);

// exit
int		exit_shell(t_list **lst, t_list **export, t_utils *cmd_utils, t_token ***tokens, char *line);
void	free_split(char **str);

// free utils
// void	free_exit(t_list **head);
void	del_node(void *node);
#endif