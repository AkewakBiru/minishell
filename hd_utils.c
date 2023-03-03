/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 16:10:28 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 16:13:49 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
