/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/02/17 19:01:58 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	free_exit(t_list **head)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = (*head)->next;
	tmp2 = (*head)->next;
	while (tmp->next)
	{
		tmp = tmp->next;
		free(tmp2);
		tmp2 = tmp;
	}
	free(tmp);
	exit(1);
}

int	is_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	del_node(t_list *node)
{
	free(((t_dict *)node->content)->key);
	free(((t_dict *)node->content)->value);
	free(((t_dict *)node->content));
	return ;
}

void	exec_builtin(t_utils *cmd_utils, t_list **lst, t_list **export)
{
	if (ft_strcmp(cmd_utils->cmd, "env") == 0)
		print_env(lst);
	else if (ft_strcmp(cmd_utils->cmd, "export") == 0)
		export_bltin(lst, cmd_utils, export);
	else if (ft_strcmp(cmd_utils->cmd, "pwd") == 0)
		print_pwd();
	else if (ft_strcmp(cmd_utils->cmd, "cd") == 0)
		chg_dir(cmd_utils, lst, export);
	else if (ft_strcmp(cmd_utils->cmd, "unset") == 0)
		unset_builtin(cmd_utils, lst, export);
	else if (ft_strcmp(cmd_utils->cmd, "echo") == 0)
		echo(cmd_utils->cmd_arg);
}

void	exec_pipex(t_utils *cmd_utils, t_list **lst)
{
	(void)lst;
	(void)cmd_utils;
	printf("pipex funcs go here\n");
}

void	exec_cmd(t_utils *cmd_utils, t_list **lst, t_list **export)
{
	if (is_builtin(cmd_utils->cmd) == 1)
		exec_builtin(cmd_utils, lst, export);
	else
		exec_pipex(cmd_utils, lst);
}

int	main(int ac, char **av, char **envp)
{
	// t_list	*tokens;
	t_token	*tokens;
	t_list	*lst;
	t_utils	cmd_utils;
	t_list	*export;
	char	*line;

	lst = NULL;
	export = NULL;
	(void)av;
	if (ac != 1)
		return (1);
	create_env(&lst, envp);
	create_env(&export, envp);
	cmd_utils.flag = 1;
	cmd_utils.pwd = 0;
	while (1)
	{
		// tokens = NULL;
		line = readline("Minishell> ");
		if (!line)
			free_exit(&lst);
		tokens = parse(line);
		//if (!tokens)
		//{
			// print error message and continue
		// }
		// t_list *node = tokens;
		// while(node)
		// {
		// 	printf("%s\n", ((t_token *)node->content)->token);
		// 	node = node->next;
		// }
		cmd_utils.cmd_arg = ft_split(line, ' ');
		if (ft_strcmp(line, "") == 0)
			continue ;
		cmd_utils.cmd = cmd_utils.cmd_arg[0];
		if (cmd_utils.cmd_arg && cmd_utils.cmd_arg[0] && ft_strcmp(cmd_utils.cmd_arg[0], "export") == 0)
		{
			if (cmd_utils.cmd_arg[1])
			{
				if (ft_strchr(cmd_utils.cmd_arg[1], '='))
					cmd_utils.flag = 1;
				else
					cmd_utils.flag = 0;
				cmd_utils.key = ft_strndup(cmd_utils.cmd_arg[1], '=');
				if (ft_strchr(cmd_utils.cmd_arg[1], '='))
					cmd_utils.value = ft_strdup(ft_strchr(cmd_utils.cmd_arg[1], '=') + 1);
				else
					cmd_utils.value = ft_strdup("");
			}
		}
		exec_cmd(&cmd_utils, &lst, &export);
		free(line);
		ft_lstclear(&tokens, free);
	}
}
