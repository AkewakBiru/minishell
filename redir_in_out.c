/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in_out.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 21:34:23 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 22:05:53 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_left_pipe(t_token **tokens, int i)
{
	int	j;

	j = i - 1;
	while (j > 0 && tokens + j && tokens[j] && tokens[j]->type != pip)
		j--;
	return (j);
}

int	get_right_pipe(t_token **tokens, int i)
{
	int	j;

	j = i;
	if (tokens + i == 0)
		return (-1);
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
		j++;
	if (tokens + j && tokens[j] && tokens[j]->type == pip)
		return (++j);
	return (j);
}

int	redir_error(char *msg, char *file, int err)
{
	ft_putstr_fd("bash: ", 2);
	if (file)
		ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (err);
}

int	redir(t_token **tokens, t_ints *t_int)
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
