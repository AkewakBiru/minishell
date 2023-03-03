/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 21:14:27 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 15:25:00 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_arg_size(t_token **tokens, int j)
{
	int	i;

	i = 0;
	while (tokens + j && tokens[j] && (tokens[j]->type == option
			|| tokens[j]->type == arg))
	{
		i++;
		j++;
	}
	return (i);
}

int	count_args(t_token **tokens, int *i)
{
	int	j;
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

int	get_nearest_pip_cmd(t_token **tokens, int i)
{
	int	j;

	j = ++i;
	while (tokens + j && tokens[j] && tokens[j]->type != pip
		&& tokens[j]->type != cmd)
		j++;
	return (j);
}

int	check_file(char *file, int j)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-5);
	close(fd);
	return (j);
}
