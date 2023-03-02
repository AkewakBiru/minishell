/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:10:23 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 03:13:30 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal2(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(2, "\n", 1);
	close(STDIN_FILENO);
}

int	create_hd_file(int num, int flag)
{
	int		hd;
	char	*hdoc_name;
	char	*h_num;

	h_num = ft_itoa(num);
	hdoc_name = ft_strjoin(".heredoc", h_num);
	free(h_num);
	if (flag == 1)
		hd = open(hdoc_name, O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	else
		hd = open(hdoc_name, O_RDONLY, 0000644);
	free(hdoc_name);
	if (hd < 0)
	{
		perror("");
		return (-1);
	}
	return (hd);
}

void	sig_ignore(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
}

int	heredoc(int num, t_token *token, t_list *lst, t_ints *t_int)
{
	int		hd;
	char	*tmp;
	char	*expanded;

	hd = create_hd_file(num, 1);
	if (hd < 0)
		return (0);
	if (signal(SIGINT, handle_signal2) != SIG_ERR)
		t_int->e_status = 1;
	if (signal(SIGQUIT, SIG_IGN) != SIG_ERR)
		t_int->e_status = 0;
	while (1)
	{
		write(1, "> ", 2);
		tmp = get_next_line(0);
		if (!tmp)
			return (0);
		if (ft_strncmp(tmp, token->token, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			break ;
		}
		expanded = expand(token, lst, t_int);
		if (expanded)
		{
			write(hd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(hd, tmp, ft_strlen(tmp));
		free(tmp);
	}
	close(hd);
	return (0);
}

void	do_heredoc(t_token **tokens, t_list *env_pack[2], t_ints *t_int)
{
	int		i;
	int		j;
	char	*lim;

	i = 0;
	j = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == here_doc)
		{
			lim = tokens[i+1]->token;
			tokens[i+1]->token = ft_strjoin(tokens[i+1]->token, "\n");
			free(lim);
			heredoc(i, tokens[i+1], env_pack[0], t_int);
			j++;
		}
		i++;
	}
}

void	rm_hd_files(t_token **tokens)
{
	int		i;
	int		j;
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
