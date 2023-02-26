/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/02/26 21:41:49 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_signal(int sig)
{
	(void)sig;
	// means we have moved into a newline
	rl_on_new_line();
	// redisplay the prompt (after the changes are made)
	rl_redisplay();
	write(1, "  \n", 3);
	// add whatever text is given as first argument to the char* buffer, in this case empty string
	// rl_replace_line("", 0);
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
		if (line[i] != ' ' || line[i] != '\t' || line[i] != '\n'
			|| line[i] != '\v' || line[i] != '\f' || line[i] != '\r')
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

int count_cmd_nums(t_token	**tokens)
{
	int i = 0;
	int count = 0;

	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == cmd)
			count++;
		i++;
	}
	return (count);
}

void	free_cmd_params(t_cmd_op **cmds)
{
	int	i;
	
	i = 0;
	while (cmds + i && cmds[i])
	{
		free(cmds[i]->cmd);
		free(cmds[i]->cmd_args);
		free(cmds[i]);
		i++;
	}
	free(cmds);
}

int	main(int ac, char **av, char **envp)
{
	t_token	**tokens;
	t_list	*lst;
	t_list	*export;
	char	*line;
	t_strs *cmd_list;
	t_cmd_op	**cmds;
	int			exit_status;

	lst = NULL;
	export = NULL;
	(void)av;
	if (ac != 1)
		return (1);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	create_env(&lst, envp);
	create_env(&export, envp);
	exit_status = 0;
	while (1)
	{
		line = readline("Minishell> ");
		if (!line)
		{
			ft_lstclear_dict(&lst, free);
			ft_lstclear_dict(&export, free);
			ft_putendl_fd("exit", 2);
			return (0);
		}
		if (!ft_strlen(line))
		{
			free(line);
			continue;
		}
		add_history(line);
		tokens = parse(line, lst);
		if (!tokens)
		{
			printf("Syntax error!\n");
			free(line);
			exit_status = 2;
			continue;
		}
		cmd_list = init_struct();
		cmd_list->env_p = ft_strdup(get_val(&lst, "PATH"));
		cmd_list->ind_p = ft_split(cmd_list->env_p, ':');
		cmd_list->cmd_len = count_cmd_nums(tokens);
		cmds = create_cmd_list(&cmd_list, tokens, &lst);
		executor(cmds, &lst, &export, tokens, line);
		free(cmd_list->env_p);
		cmd_list->env_p = 0;
		free_arr(cmd_list->ind_p);
		cmd_list->ind_p = 0;
		free(cmd_list);
		cmd_list = 0;
		free_cmd_params(cmds);
		cmds = 0;
		free(line);
		free_tokens(&tokens);
	}
	return (exit_status);
}
