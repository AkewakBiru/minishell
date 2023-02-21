/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/02/21 10:58:39 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	del_node(void *node)
{
	free(((t_dict *)((t_list *)node)->content)->key);
	free(((t_dict *)((t_list *)node)->content)->value);
	free(((t_dict *)((t_list *)node)->content));
	return ;
}

/*
* Added "*" before tokens because of compilation error
*/
void	exec_builtin(t_utils *cmd_utils, t_list **lst, t_list **export, t_token ***tokens, char *line)
{
	if (!ft_strcmp(cmd_utils->cmd, "env"))
		print_env(lst);
	else if (!ft_strcmp(cmd_utils->cmd, "export"))
		export_bltin(lst, cmd_utils, export);
	else if (!ft_strcmp(cmd_utils->cmd, "pwd"))
		print_pwd();
	else if (!ft_strcmp(cmd_utils->cmd, "cd"))
		chg_dir(cmd_utils, lst, export);
	else if (!ft_strcmp(cmd_utils->cmd, "unset"))
		unset_builtin(cmd_utils, lst, export);
	else if (!ft_strcmp(cmd_utils->cmd, "exit"))
		exit_shell(lst, export, cmd_utils, tokens, line);
	else if (!ft_strcmp(cmd_utils->cmd, "echo"))
		echo(cmd_utils->cmd_arg);
}

void	exec_pipex(t_utils *cmd_utils, t_list **lst)
{
	(void)lst;
	(void)cmd_utils;
	printf("pipex funcs go here\n");
}

void	exec_cmd(t_utils *cmd_utils, t_list **lst, t_list **export, t_token ***tokens, char *line)
{
	if (is_builtin(cmd_utils->cmd) == 1)
		exec_builtin(cmd_utils, lst, export, tokens, line);
	else
		exec_pipex(cmd_utils, lst);
}

void	handle_signal(int sig)
{
	(void)sig;
	// means we have moved into a newline
	rl_on_new_line();
	// redisplay the prompt (after the changes are made)
	rl_redisplay();
	write(1, "  \n", 3);
	// add whatever text is given as first argument to the char* buffer, in this case empty string
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	check_line(char *line)
{
	int	i;
	
	if (!line)
		return (0);
	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ')
			return (1);
	}
	return (0);
}

void	free_tokens(t_token ***tokens_p)
{
	int		i;

	i = 0;
	while ((*tokens_p)[i])
	{
		free((*tokens_p)[i]->token);
		free((*tokens_p)[i]);
		i++;
	}
	free((*tokens_p));
}

int	main(int ac, char **av, char **envp)
{
	// t_list	*tokens;
	t_token	**tokens;
	t_list	*lst;
	t_utils	cmd_utils;
	t_list	*export;
	char	*line;

	lst = NULL;
	export = NULL;
	(void)av;
	if (ac != 1)
		return (1);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	create_env(&lst, envp);
	create_env(&export, envp);
	cmd_utils.flag = 1;
	cmd_utils.pwd = 0;
	cmd_utils.EXIT_STATUS = 0;
	while (1)
	{
		// tokens = NULL;
		cmd_utils.key = 0;
		cmd_utils.value = 0;
		line = readline("Minishell> ");
		if (!line)
		{
			ft_lstclear_dict(&lst, free);
			ft_lstclear_dict(&export, free);
			return (0);
		}
		if (check_line(line))
			add_history(line);
		tokens = parse(line, lst);
		if (!tokens)
		{
			free(line);
			continue;
		}
		// int i = 0;
		// while (tokens[i])
		// {
		// 	printf("token: %s, type: %u\n", tokens[i]->token, tokens[i]->type);
		// 	i++;
		// }
		cmd_utils.cmd_arg = ft_split(line, ' ');
		if (ft_strcmp(line, "") == 0)
		{
			free(line);
			free_split(cmd_utils.cmd_arg);
			// ft_lstclear(&tokens, free);
			continue ;
		}
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

		exec_cmd(&cmd_utils, &lst, &export, &tokens, line);
		free(line);
		free_split(cmd_utils.cmd_arg);
		if (cmd_utils.key != 0)
			free(cmd_utils.key);
		if (cmd_utils.value != 0)
			free(cmd_utils.value);
		free_tokens(&tokens);
		// ft_lstclear(&tokens, free);
	}
}
