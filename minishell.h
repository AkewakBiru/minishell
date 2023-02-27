/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:58:30 by abiru             #+#    #+#             */
/*   Updated: 2023/02/27 15:27:01 by yel-touk         ###   ########.fr       */
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
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# define BUFFER_SIZE 42
# define MAX_FD 256

int	exit_status;

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
	char	*cmd;
	char	**cmd_args;
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

typedef struct strs {
	char	*env_p;
	char	**ind_p;
	char	*lim;
	char	*tmp_cmd;
	char	**cmd_args;
	char	*tmp;
	int		cmd_len;
}	t_strs;

typedef struct	s_ints
{
	int	counter;
	int	cmd_count;
	int exit_status;
	int	RLSTDIN;
	int	RLSTDOUT;
	int	pipes[4];
}	t_ints;

// parse utils
int	is_white_space(char c);

char	*expand(char *line, t_list *lst);
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
void	export_bltin(t_list **lst, char **cmd_utils, t_list **export);
void	create_env(t_list **head, char **envp);

// cd and pwd utils
char	*get_pwd(void);
void	print_pwd(void);
int		chg_dir(char **cmd_utils, t_list **lst, t_list **export);

// linked list utils
void	sort_list(t_list **head);
void	ft_list_remove_if(t_list **head, char *str, int (*cmp)());

// echo
void	echo(char **arg);

// unset
void	unset_builtin(char **cmd_utils, t_list **lst, t_list **export);

// exit
int exit_shell(t_list **lst, t_list **export, char **cmd_args, t_token **tokens, t_ints *t_int, char *line);
void	free_split(char **str);

// free utils
// void	free_exit(t_list **head);
void	del_node(void *node);
void	free_cmd_params(t_cmd_op **cmds);

t_cmd_op	**create_cmd_list(t_strs **cmd_list, t_token **tokens, t_list **lst);

// starts execution
void	executor(t_cmd_op **cmds, t_list **lst, t_list **export, t_token **tokens, char *line);

// pipex utils
char	*get_envp(char **envp);
char	*get_cmd_path(char **env, char *cmd);
char	**sep_cmdopt(char *str);
t_strs	*init_struct(void);
void	free_arr(char **arr);

// getnextline utils
char	*get_next_line(int fd);


//heredoc utils
void	rm_hd_files(t_token **tokens);
void	do_heredoc(t_token **tokens);
int		heredoc(int num, char *delim);

int	is_builtin(char *cmd);

void	custom_err_msg(char *cmd, char *arg);
int	check_key_names(char *cmd, char **cmd_utils);
#endif