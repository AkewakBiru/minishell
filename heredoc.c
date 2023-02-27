/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:10:23 by abiru             #+#    #+#             */
/*   Updated: 2023/02/27 23:25:08 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_signal2(int sig)
{
	(void)sig;
	rl_on_new_line();
	// rl_redisplay();
	ft_putstr_fd("    \b\b\b\b", 2);
	write(2, ">\n", 2);
	close(STDIN_FILENO);
}

int	heredoc(int num, char *delim)
{
	char	*hdoc_name;
	int		hd;
	char	*tmp;
	char	*hnum;
	char	*lim;

	hnum = ft_itoa(num);
	hdoc_name = ft_strjoin(".heredoc", hnum);
	hd = open(hdoc_name, O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	free(hnum);
	free(hdoc_name);
	if (hd < 0)
		return (0);
	signal(SIGINT, handle_signal2);
	signal(SIGQUIT, SIG_IGN);
	lim = ft_strjoin(delim, "\n");
	while (1)
	{
		write(1, "> ", 2);
		tmp = get_next_line(0);
		if (!tmp)
		{
			free(lim);
			return (0);
		}
		if (ft_strncmp(tmp, lim, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			break ;
		}
		write(hd, tmp, ft_strlen(tmp));
		free(tmp);
	}
	close(hd);
	free(lim);
	return (0);
}

void	do_heredoc(t_token **tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == here_doc)
		{
			heredoc(i, tokens[i + 1]->token);
			j++;
		}
		i++;
	}
}

void	rm_hd_files(t_token **tokens)
{
	int	i;
	int	j;
	char	*fname;
	char	*num;

	i = 0;
	j = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == here_doc)
		{
			num = ft_itoa(i);
			fname = ft_strjoin(".heredoc", num);
			if (!access(fname, F_OK))
				unlink(fname);
			free(num);
			free(fname);
			j++;
		}
		i++;
	}
}
