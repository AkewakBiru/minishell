/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 18:39:24 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 22:06:06 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_hd_err(t_token **tokens, int j, int flag, t_ints *t_int)
{
	char	*hdname;
	char	*hdnum;
	int		k;

	(void)tokens;
	hdnum = ft_itoa(j);
	hdname = ft_strjoin(".heredoc", hdnum);
	free(hdnum);
	k = open(hdname, O_RDONLY);
	free(hdname);
	if (k < 0)
	{
		if (!flag)
			t_int->e_status = redir_error("heredoc file error", "", 1);
		return (-1);
	}
	close(k);
	return (0);
}

static int	check_rin_err(t_token **tokens, int j, int flag, t_ints *t_int)
{
	int	k;

	k = open(tokens[j + 1]->token, O_RDONLY);
	if (k < 0)
	{
		if (!flag)
			t_int->e_status = redir_error(strerror(errno),
					tokens[j + 1]->token, 1);
		return (-1);
	}
	close(k);
	return (0);
}

int	do_in_redir(t_token **tokens, int i, int flag, t_ints *t_int)
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
		if (tokens + j && tokens[j] && tokens[j]->type == redir_in)
		{
			if (check_rin_err(tokens, j, flag, t_int) == -1)
				return (-1);
		}
		else if (tokens + j && tokens[j] && tokens[j]->type == here_doc)
		{
			if (check_hd_err(tokens, j, flag, t_int) == -1)
				return (-1);
		}
		j++;
	}
	return (0);
}
