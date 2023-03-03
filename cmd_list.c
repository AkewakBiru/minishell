/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:29:12 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 16:48:06 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	returns 0 if the stdin is default, positive num if the stdin is redirection, 
	and negative num if the stdin is a pipe.
*/

int	find_stdin2(t_token	**tokens, int j)
{
	if (tokens[j]->type == redir_in)
		return (check_file(tokens[j + 1]->token, j));
	return (j);
}

int	find_stdin(t_token	**tokens, int i)
{
	int	j;

	j = get_nearest_pip_cmd(tokens, i);
	if (tokens[j] == 0 || tokens[j]->type == pip || tokens[j]->type == cmd)
		j--;
	while (j > i)
	{
		if (tokens[j]->type == redir_in || tokens[j]->type == here_doc)
			return (find_stdin2(tokens, j));
		j--;
	}
	j = --i;
	while (j >= 0 && tokens + j && tokens[j] && tokens[j]->type != cmd)
	{
		if (tokens[j]->type == redir_in || tokens[j]->type == here_doc)
			return (find_stdin2(tokens, j));
		else if (tokens[j]->type == pip)
			return (-1);
		j--;
	}
	return (-2);
}

int	find_stdout(t_token	**tokens, int i)
{
	int	j;

	j = get_nearest_pip_cmd(tokens, i);
	if (tokens[j] == 0 || tokens[j]->type == pip || tokens[j]->type == cmd)
		j--;
	while (j > i)
	{
		if (tokens[j]->type == redir_out || tokens[j]->type == redir_out_append)
			return (j);
		j--;
	}
	j = i - 1;
	while (j >= 0 && tokens + j && tokens[j] && tokens[j]->type != cmd
		&& tokens[j]->type != pip)
	{
		if (tokens[j]->type == redir_out || tokens[j]->type == redir_out_append)
			return (j);
		j--;
	}
	j = get_nearest_pip_cmd(tokens, i);
	if (tokens[j] != 0 && tokens[j]->type == pip)
		return (-1);
	return (-2);
}

t_cmd_op	*create_cmd(t_strs **cmd_list, t_token **tokens,
	t_cmd_op **cmds, int i)
{
	t_cmd_op	*command;

	command = (t_cmd_op *)malloc((sizeof(t_cmd_op)));
	if (!command)
	{
		ft_putendl_fd("Malloc error while creating command in command list", 2);
		free_cmd_params(cmds);
		return (0);
	}
	command->redir_in = find_stdin(tokens, i);
	command->redir_out = find_stdout(tokens, i);
	command->cmd_args = get_cmd_args(tokens, &i);
	if (command->cmd_args + 0 && is_builtin(command->cmd_args[0]))
		command->cmd = ft_strdup(command->cmd_args[0]);
	else if (command->cmd_args[0] == 0 || !ft_strcmp(command->cmd_args[0], ""))
		command->cmd = ft_strdup("");
	else if ((ft_strchr(command->cmd_args[0], '/'))
		|| (command->cmd_args[0] && !(*cmd_list)->env_p))
		command->cmd = ft_strdup(command->cmd_args[0]);
	else if ((*cmd_list)->env_p)
		command->cmd = get_cmd_path((*cmd_list)->ind_p,
				tokens[i]->token);
	return (command);
}

t_cmd_op	**create_cmd_list(t_strs **cmd_list, t_token **tokens)
{
	int			i;
	int			j;
	t_cmd_op	**cmds;

	i = 0;
	j = 0;
	cmds = (t_cmd_op **)malloc(sizeof(t_cmd_op *) * ((*cmd_list)->cmd_len + 1));
	if (!cmds)
	{
		ft_putendl_fd("Malloc error while creating command list", 2);
		return (0);
	}
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == cmd)
		{
			cmds[j] = create_cmd(cmd_list, tokens, cmds, i);
			if (!cmds[j++])
				return (0);
		}
		i++;
	}
	cmds[j] = 0;
	return (cmds);
}
