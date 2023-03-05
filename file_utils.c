/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 20:25:35 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:58:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_infile(t_cmd_op **cmds, t_ints *t_int, t_token **tokens)
{
	int	infile;

	infile = 0;
	if (cmds[t_int->counter]->redir_in != -2
		&& cmds[t_int->counter]->redir_in != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_in]->type == redir_in)
			infile = open(tokens[cmds[t_int->counter]
					->redir_in + 1]->token, O_RDONLY);
		else if (tokens[cmds[t_int->counter]->redir_in]->type == here_doc)
			infile = create_hd_file(cmds[t_int->counter]->redir_in, 0);
		if (infile < 0)
		{
			perror("");
			return (-1);
		}
	}
	return (infile);
}

int	open_outfile(t_cmd_op **cmds, t_ints *t_int, t_token **tokens)
{
	int	outfile;

	outfile = 0;
	if (cmds[t_int->counter]->redir_out != -2
		&& cmds[t_int->counter]->redir_out != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_out]->type == redir_out)
			outfile = open(tokens[cmds[t_int->counter]->redir_out + 1]
					->token, O_RDWR | O_CREAT | O_TRUNC, 0000644);
		else if (tokens[cmds[t_int->counter]->redir_out]
			->type == redir_out_append)
			outfile = open(tokens[cmds[t_int->counter]->redir_out + 1]
					->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
		if (outfile < 0)
		{
			perror("");
			return (-1);
		}
	}
	return (outfile);
}
