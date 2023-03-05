/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 10:43:37 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 17:01:40 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_redirection(t_token **tokens, t_ints *t_int, int i)
{
	while (tokens + i && tokens[i] && tokens[i]->type != pip)
	{
		if (tokens[i]->type == redir_in)
		{
			if (check_rin_err(tokens, i, 0, t_int) == -1)
				return (-5);
		}
		else if (tokens[i]->type == here_doc)
		{
			if (check_hd_err(tokens, i, 0, t_int) == -1)
				return (-5);
		}
		else if (tokens[i]->type == redir_out)
		{
			if (check_rout(tokens, i, 0, t_int) == -1)
				return (-5);
		}
		else if (tokens[i]->type == redir_out_append)
		{
			if (check_rout_app(tokens, i, 0, t_int) == -1)
				return (-5);
		}
		i++;
	}
	return (0);
}

int	go_to_next_pip(t_token **tokens, int i)
{
	int	j;

	j = i;
	while (tokens + j && tokens[j])
	{
		if (tokens[j]->type != pip)
			j++;
		else
			break ;
	}
	if (tokens + j && tokens[j] && tokens[j]->type == pip)
		j++;
	return (j);
}

int	find_cmd(t_token **tokens, int i)
{
	while (tokens + i && tokens[i] && tokens[i]->type != pip)
	{
		if (tokens[i]->type == cmd)
			return (i);
		i++;
	}
	return (-1);
}

void	update_status_counter(t_ints *t_int)
{
	t_int->e_status = 1;
	t_int->counter++;
}
