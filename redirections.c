/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/05 10:25:07 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:29:26 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_hd_err(t_token **tokens, int j, int flag, t_ints *t_int)
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

int	check_rin_err(t_token **tokens, int j, int flag, t_ints *t_int)
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

int	check_rout(t_token **tokens, int j, int flag, t_ints *t_int)
{
	int	k;

	k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_TRUNC, 0000644);
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

int	check_rout_app(t_token **tokens, int j, int flag, t_ints *t_int)
{
	int	k;

	k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
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

int	redir_error(char *msg, char *file, int err)
{
	ft_putstr_fd("bash: ", 2);
	if (file)
		ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (err);
}
