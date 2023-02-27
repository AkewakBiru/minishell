/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:34:23 by abiru             #+#    #+#             */
/*   Updated: 2023/02/27 23:28:51 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_left_pipe(t_token **tokens, int i)
{
	int j;

	j = i - 1;
	while (j > 0 && tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		j--;
	}
	return (j);
}

int	do_in_redir(t_token **tokens, int i)
{
	int	j;
	int	k;

	if (i > 0)
		j = get_left_pipe(tokens, i);
	else
		j = 0;
	if (tokens + j && tokens[j] && (tokens[j]->type == pip || tokens[j]->type == cmd))
		j++;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens + j && tokens[j] && tokens[j]->type == redir_in)
		{
			k = open(tokens[j + 1]->token, O_RDONLY);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
}

int	do_out_redir(t_token **tokens, int i)
{
	int	j;
	int	k;
	if (i > 0)
		j = get_left_pipe(tokens, i);
	else
		j = 0;
	if (tokens + j && tokens[j] && (tokens[j]->type == pip || tokens[j]->type == cmd))
		j++;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens + j && tokens[j] && tokens[j]->type == redir_out)
		{
			k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_TRUNC, 0000644);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		else if (tokens + j && tokens[j] && tokens[j]->type == redir_out_append)
		{
			k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
}
