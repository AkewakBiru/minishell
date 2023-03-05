/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_out.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:43:56 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:26:34 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	do_out_redir(t_token **tokens, int i, int flag, t_ints *t_int)
{
	int	j;

	j = 0;
	if (i > 0)
		j = get_left_pipe(tokens, i);
	if (tokens + j && tokens[j] && (tokens[j]->type == pip
			|| tokens[j]->type == cmd))
		j++;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens + j && tokens[j] && tokens[j]->type == redir_out)
		{
			if (check_rout(tokens, j, flag, t_int) == -1)
				return (-1);
		}
		else if (tokens + j && tokens[j] && tokens[j]->type == redir_out_append)
		{
			if (check_rout_app(tokens, j, flag, t_int) == -1)
				return (-1);
		}
		j++;
	}
	return (0);
}
