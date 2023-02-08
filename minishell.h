/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:58:30 by abiru             #+#    #+#             */
/*   Updated: 2023/02/08 14:05:43 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"

enum e_input_type
{
	cmd,
	option,
	arg,
	pip,
	file,
	redir_in,
	redir_out,
	here_doc,
	redir_out_append,
	dollar,
	s_quote,
	d_quote
};

typedef struct s_token
{
	char				*token;
	enum e_input_type	type;
}	t_token;

typedef struct cmd_utils
{
	char	*cmd;
	char	**cmd_arg;
	char	*key;
	char	*value;
	int		flag;
}	t_utils;

void	parse(char *line, t_list **tokens);

// dictionary utils
void	free_dict(t_dict	*dict);
int		key_exists(char *s, t_list **new);
t_dict	*create_dict(char *key, char *value, int flag);
int		update_dict(t_utils *cmd, t_list **head);

// export and env utils
void	print_env(t_list **lst);
void	print_list(t_list **lst);
void	update_env(t_utils *cmd, t_list **env);
void	export_bltin(t_list **lst, t_utils *cmd_utils, t_list **export);
void	create_env(t_list **head, char **envp);

// linked list utils
void	sort_list(t_list **head);

// free utils
void	free_exit(t_list **head);
#endif