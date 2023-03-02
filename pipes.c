/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 19:17:43 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 22:05:42 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*create_pipes(t_ints *t_int)
{
	int	*pipes;
	int	j;

	pipes = (int *)malloc(sizeof(int) * (t_int->pip_count) * 2);
	if (!pipes)
		return (0);
	j = -1;
	while (++j < t_int->pip_count)
	{
		if (pipe(pipes + 2 * j) < 0)
		{
			perror("");
			return (0);
		}
	}
	return (pipes);
}

int	count_pipes(t_token	**tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == pip)
			count++;
		i++;
	}
	return (count);
}

void	close_pipes(t_ints *t_int)
{
	int	i;

	i = -1;
	while (++i < (t_int->pip_count) * 2)
		close(t_int->pipes[i]);
}
