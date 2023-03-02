/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:29:12 by abiru             #+#    #+#             */
/*   Updated: 2023/03/01 20:16:11 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_arg_size(t_token **tokens, int j)
{
	int	i;

	i = 0;
	while (tokens + j && tokens[j] && (tokens[j]->type == option || tokens[j]->type == arg))
	{
		i++;
		j++;
	}
	return (i);
}

// char	*join_and_free(char *s1, char *s2)
// {
// 	char *tmp;
// 	char *new;

// 	tmp = ft_strjoin(s1, " ");
// 	free(s1);
// 	new = ft_strjoin(tmp, s2);
// 	free(s2);
// 	return (new);
// }

int	count_args(t_token **tokens, int *i)
{
	int j;
	int	count;

	j = *i;
	count = 0;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens[j]->type == option || tokens[j]->type == 2)
			count++;
		j++;
	}
	return (count);
}

char	**get_cmd_args(t_token **tokens, int *i)
{
	char	**arg;
	int		count;
	int		j;
	int		k;

	count = count_args(tokens, i);
	arg = (char **)malloc(sizeof(char *) * (count + 2));
	if (!arg)
		return (0);
	j = *i;
	k = 0;
	arg[k++] = ft_strdup(tokens[j++]->token);
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens[j]->type == option || tokens[j]->type == 2)
			arg[k++] = ft_strdup(tokens[j]->token);
		j++;
	}
	arg[k] = 0;
	return (arg);
}

/*
	returns 0 if the stdin is default, positive num if the stdin is redirection, and negative num
	if the stdin is a pipe.
*/

int get_nearest_pip_cmd(t_token **tokens, int i)
{
	int	j;

	j = ++i;
	while (tokens + j && tokens[j] && tokens[j]->type != pip && tokens[j]->type != cmd)
		j++;
	return (j);
}

/*
< a < b < c ls < e < f < g - should use g as its stdin.
*/

int check_file(char *file, int j)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-5);
	close(fd);
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
		{
			if (tokens[j]->type == redir_in)
				return (check_file(tokens[j + 1]->token, j));
			return (j);
		}
		j--;
	}
	j = --i;
	while (j >= 0 && tokens + j && tokens[j] && tokens[j]->type != cmd)
	{
		if (tokens[j]->type == redir_in || tokens[j]->type == here_doc)
		{
			if (tokens[j]->type == redir_in)
				return (check_file(tokens[j + 1]->token, j));
			return (j);
		}
		else if (tokens[j]->type == pip)
			return (-1);
		j--;
	}
	return (-2);
}

int find_stdout(t_token	**tokens, int i)
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
	while (j >= 0 && tokens + j && tokens[j] && tokens[j]->type != cmd && tokens[j]->type != pip)
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

// void	free_siblings(t_cmd_op **cmds)
// {
// 	int	i;

// 	i = 0;
// 	while (cmds && cmds + i && cmds[i])
// 	{
// 		free(cmds[i]);
// 		cmds[i] = 0;
// 		i++;
// 	}
// 	if (cmds)
// 		free(cmds);
// 	cmds = 0;
// }

t_cmd_op	**create_cmd_list(t_strs **cmd_list, t_token **tokens)
{
	int i = 0;
	int j = 0;
	t_cmd_op	**cmds;

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
			cmds[j] =(t_cmd_op *)malloc((sizeof(t_cmd_op)));
			if (!cmds[j])
			{
				free_cmd_params(cmds);
				return (0);
			}
				// free all of the list created above also
			cmds[j]->redir_in = find_stdin(tokens, i);
			cmds[j]->redir_out = find_stdout(tokens, i);
			cmds[j]->cmd_args = get_cmd_args(tokens, &i);
			if (cmds[j]->cmd_args + 0 && is_builtin(cmds[j]->cmd_args[0]))
				cmds[j]->cmd = ft_strdup(cmds[j]->cmd_args[0]);
			else if (cmds[j]->cmd_args[0] == 0)
				cmds[j]->cmd = ft_strdup("");
			else if ((ft_strchr(cmds[j]->cmd_args[0], '/'))
			|| (cmds[j]->cmd_args[0] && !(*cmd_list)->env_p))
				cmds[j]->cmd = ft_strdup(cmds[j]->cmd_args[0]);
			else if ((*cmd_list)->env_p)
				cmds[j]->cmd = get_cmd_path((*cmd_list)->ind_p, tokens[i]->token);
			j++;
		}
		i++;
	}
	cmds[j] = 0;
	return (cmds);
}
