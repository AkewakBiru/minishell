/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 17:10:23 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 13:52:18 by abiru            ###   ########.fr       */
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

// void	sig_ignore(int sig)
// {
// 	(void)sig;
// 	rl_replace_line("", 0);
// }

int	init_hd(t_ints	*t_int, int num)
{
	int	hd;

	if (signal(SIGINT, handle_signal2) != SIG_ERR)
		t_int->e_status = 1;
	if (signal(SIGQUIT, SIG_IGN) != SIG_ERR)
		t_int->e_status = 0;
	hd = create_hd_file(num, 1);
	if (hd < 0)
		return (-1);
	return (hd);
}

void	write_to_file(t_token **token, t_list *lst, t_ints *t_int, char *tmp)
{
	char	*expanded;

	(*token)->token = ft_strdup(tmp);
	expanded = expand(token, lst, t_int);
	if (expanded)
	{
		write(t_int->hd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(t_int->hd, tmp, ft_strlen(tmp));
	free(tmp);
	free((*token)->token);
}

/*
	needs to be checked for leaks
*/
int	heredoc(int num, t_token **token, t_list *lst, t_ints *t_int)
{
	char	*tmp;
	char	*lim;

	t_int->hd = init_hd(t_int, num);
	if (t_int->hd == -1)
		return (0);
	lim = ft_strdup((*token)->token);
	while (1)
	{
		write(1, "> ", 2);
		tmp = get_next_line(0);
		if (!tmp)
			return (close(t_int->hd), free(lim), 0);
		if (ft_strncmp(tmp, lim, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			break ;
		}
		write_to_file(token, lst, t_int, tmp);
	}
	// (*token)->token = ft_strdup("");
	return (close(t_int->hd), free(lim), 0);
}

void	do_heredoc(t_token **tokens, t_list *env_pack[2], t_ints *t_int)
{
	int		i;
	int		j;
	char	*lim;
	t_token	*tok;

	i = 0;
	j = 0;
	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		return ;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == here_doc)
		{
			lim = ft_strdup(tokens[i + 1]->token);
			tok->token = ft_strjoin(lim, "\n");
			free(lim);
			heredoc(i, &tok, env_pack[0], t_int);
			j++;
		}
		i++;
	}
	free(tok);
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
