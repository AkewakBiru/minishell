/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:58:30 by abiru             #+#    #+#             */
/*   Updated: 2023/02/06 20:58:20 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "./libft/libft.h"

typedef struct cmd_utils
{
	char	*cmd;
	char	**cmd_arg;
	char	*key;
	char	*value;
	int		flag;
}	t_utils;

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