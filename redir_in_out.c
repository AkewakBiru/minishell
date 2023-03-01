/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:34:23 by abiru             #+#    #+#             */
/*   Updated: 2023/03/01 20:39:00 by abiru            ###   ########.fr       */
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

int	do_in_redir(t_token **tokens, int i, int flag, t_ints *t_int)
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
				if (!flag)
				{
					error_msg(strerror(errno), &tokens[j + 1]->token, 1, 1);
					t_int->e_status = 1;
				}
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
}

int	do_out_redir(t_token **tokens, int i, int flag, t_ints *t_int)
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
				if (!flag)
				{
					error_msg(strerror(errno), &tokens[j + 1]->token, 1, 1);
					t_int->e_status = 1;
				}
				return (-1);
			}
			close(k);
		}
		else if (tokens + j && tokens[j] && tokens[j]->type == redir_out_append)
		{
			k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
			if (k < 0)
			{
				if (!flag)
				{
					error_msg(strerror(errno), &tokens[j + 1]->token, 1, 1);
					t_int->e_status = 1;
				}
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
}

int	get_right_pipe(t_token **tokens, int i)
{
	int j;

	j = i;
	if (tokens + i == 0)
		return (-1);
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		j++;
	}
	if (tokens + j && tokens[j] && tokens[j]->type == pip)
		return (++j);
	return (j);
}

int redir(t_token **tokens, t_ints *t_int)
{
	int	i;

	i = 0;
	if (!(tokens + 0) || !tokens[0])
		return (0);
	while (tokens + i != 0 && tokens[i] != 0)
	{
		if (do_in_redir(tokens, i, 0, t_int) == -1)
		{
			i = get_right_pipe(tokens, i);
			t_int->e_status = 1;
			continue ;
		}
		if (do_out_redir(tokens, i, 0, t_int) == -1)
			t_int->e_status = 0;
		i = get_right_pipe(tokens, i);
		if (i < 0 || !(tokens + i))
			return (1);
	}
	return (0);
}
