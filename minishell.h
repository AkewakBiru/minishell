/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:58:30 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 14:54:36 by abiru            ###   ########.fr       */
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
# include <errno.h>
# include <dirent.h>
# define BUFFER_SIZE 42
# define MAX_FD 256

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
	delimiter_q,
	empty_expansion,
	unset,
	redirection
};

typedef struct s_token
{
	char				*token;
	enum e_input_type	type;
}	t_token;

typedef struct cmd_op
{
	char	*cmd;
	char	**cmd_args;
	int		redir_in;
	int		redir_out;
}	t_cmd_op;

typedef struct strs {
	char	*env_p;
	char	**ind_p;
	char	*lim;
	char	*tmp_cmd;
	char	**cmd_args;
	char	*tmp;
	int		cmd_len;
}	t_strs;

typedef struct s_ints
{
	int	counter;
	int	cmd_count;
	int	pip_count;
	int	rlstdin;
	int	rlstdout;
	int	*pipes;
	int	e_status;
	int	child_id;
	int	hd;
}	t_ints;

// parse utils
int			is_white_space(char c);
int			should_expand(t_token ***tokens, int index, char *line);

char		*expand(t_token **token, t_list *lst, t_ints *t_int);
void		check_quotes(char c, int *s_quote, int *d_quote);
void		free_tokens(t_token ***tokens_p);
t_token		**parse(char *line, t_list *lst, t_ints *t_int);
int			label_tokens(t_token ***tokens);
t_token		**tokenize(char const *line);

// dictionary utils
void		free_dict(t_dict	*dict, int flag);
int			key_exists(char *s, t_list **new);
t_dict		*create_dict(char *key, char *value, int flag);
int			update_dict(char *key, char *value, t_list **head);
char		*get_val(t_list **lst, char *key);

// export and env utils
void		print_env(t_list **lst, t_ints *t_int);
void		print_list(t_list **lst, t_ints *t_int);
void		update_env(t_dict *cmd, t_list **env);
void		export_bltin(t_list **lst, char **cmd_utils, t_list **export,
				t_ints *t_int);

// cd and pwd utils
char		*get_pwd(char **cmd_utils);
void		print_pwd(char **cmd_utils);
int			chg_dir(char **cmd_utils, t_list **lst, t_list **export,
				t_ints *t_int);

// linked list utils
void		sort_list(t_list **head);
void		ft_list_remove_if(t_list **head, char *str, int (*cmp)());

// echo
int			echo(char **arg);

// unset
int			unset_builtin(char **cmd_utils, t_list **lst, t_list **export);

// exit
int			exit_shell(t_list *env_pack[2], t_cmd_op **cmd, t_ints *t_int,
				int is_child);
void		free_split(char **str);
int			count_arg(char **args);

// free utils
void		del_node(void *node);
void		free_cmd_params(t_cmd_op **cmds);

t_cmd_op	**create_cmd_list(t_strs **cmd_list, t_token **tokens);

// starts execution
int			executor(t_list *env_pack[2], t_token **tokens, t_ints *t_int);
int			loop_exec_cmds(t_list *env_pack[2], t_token **tokens,
				t_cmd_op **cmds, t_ints *t_int);

// pipex utils
char		*get_envp(char **envp);
char		*get_cmd_path(char **env, char *cmd);
char		**sep_cmdopt(char *str);
t_strs		*init_struct(void);
void		free_arr(char **arr);

// getnextline utils
char		*get_next_line(int fd);

//heredoc utils
void		rm_hd_files(t_token **tokens);
void		do_heredoc(t_token **tokens, t_list *env_pack[2], t_ints *t_int);
int			heredoc(int num, t_token **delim, t_list *lst, t_ints *t_int);
int			create_hd_file(int num, int flag);

// builtin utils
int			is_builtin(char *cmd);
void		exec_builtin(t_cmd_op **cmd, t_list *env_pack[2], t_ints *t_int,
				int is_child);

int			check_key_names(char *cmd, char **cmd_utils);
void		handle_signal(int sig);

char		**construct_envp(t_list **lst);
void		create_env(t_list **head, char **envp);

// input and output redirection utils
int			redir(t_token **tokens, t_ints *t_int);
int			do_in_redir(t_token **tokens, int i, int flag, t_ints *t_int);
int			do_out_redir(t_token **tokens, int i, int flag, t_ints *t_int);
int			get_right_pipe(t_token **tokens, int i);
int			get_left_pipe(t_token **tokens, int i);
int			redir_error(char *msg, char *file, int err);

// cmd tools: cmd_count, wait
int			count_cmd_nums(t_token	**tokens);
void		reset_fd(t_ints *t_int);
int			wait_for_cmds(t_ints *t_int);
int			check_file_existence(t_token **tokens, int i, t_ints *t_int);
int			init_utils(t_token **tokens, t_ints *t_int);

// cmdlist utils
int			get_arg_size(t_token **tokens, int j);
int			count_args(t_token **tokens, int *i);
char		**get_cmd_args(t_token **tokens, int *i);
int			get_nearest_pip_cmd(t_token **tokens, int i);
int			check_file(char *file, int j);

// pipe utils
int			*create_pipes(t_ints *t_int);
int			count_pipes(t_token	**tokens);
void		close_pipes(t_ints *t_int);

int			error_msg(char *msg, char **args, int num, int err);
void		custom_err_msg(char *cmd, char *arg);
void		ex_fail_msg(t_cmd_op *cmd, char **args, t_ints *t_int);

int			update_shell(t_list *envp[2]);

// free utils
void		free_tokens(t_token ***tokens_p);
void		free_cmd_params(t_cmd_op **cmds);
void		free_split(char **str);
void		free_env_utils(t_strs	*cmd_list);
void		free_mem_child(t_cmd_op **cmds, t_list *env_pack[2], t_ints *t_int,
				t_token **tokens);

// file utils
int			open_infile(t_cmd_op **cmds, t_ints *t_int, t_token **tokens);
int			open_outfile(t_cmd_op **cmds, t_ints *t_int, t_token **tokens);
int			dup_close(t_cmd_op **cmds, t_ints *t_int, t_token **tokens);

// input redirection utils
// int	check_hd_err(t_token **tokens, int j, int flag, t_ints *t_int);
// int	check_rin_err(t_token **tokens, int j, int flag, t_ints *t_int);
// int	do_in_redir(t_token **tokens, int i, int flag, t_ints *t_int);
#endif
