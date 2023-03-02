/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 20:20:03 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 22:01:37 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_error(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
	{
		ft_putstr_fd("dup2: ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (-1);
	}
	return (0);
}

int	dup_first(t_cmd_op **cmds, t_ints *t_int, int in, int out)
{
	if (cmds[t_int->counter]->redir_in != -2)
	{
		if (dup_error(in, STDIN_FILENO) == -1)
			return (-1);
	}
	if (cmds[t_int->counter]->redir_out != -2)
	{
		if (cmds[t_int->counter]->redir_out == -1)
		{
			if (dup_error(t_int->pipes[1], STDOUT_FILENO) == -1)
				return (-1);
		}
		else
		{
			if (dup_error(out, STDOUT_FILENO) == -1)
				return (-1);
		}
	}
	return (0);
}

int	dup_middle(t_cmd_op **cmds, t_ints *t_int, int in, int out)
{
	if (cmds[t_int->counter]->redir_in == -1)
	{
		if (dup_error(t_int->pipes[2 * t_int->counter - 2], STDIN_FILENO) == -1)
			return (-1);
	}
	else
	{
		if (dup_error(in, STDIN_FILENO) == -1)
			return (-1);
	}
	if (cmds[t_int->counter]->redir_out == -1)
	{
		if (dup_error(t_int->pipes[2 * t_int->counter + 1],
				STDOUT_FILENO) == -1)
			return (-1);
	}
	else
	{
		if (dup_error(out, STDOUT_FILENO) == -1)
			return (-1);
	}
	return (0);
}

int	dup_last(t_cmd_op **cmds, t_ints *t_int, int in, int out)
{
	if (cmds[t_int->counter]->redir_in != -2)
	{
		if (cmds[t_int->counter]->redir_in == -1)
		{
			if (dup_error(t_int->pipes[2 * t_int->counter - 2],
					STDIN_FILENO) == -1)
				return (-1);
		}
		else
		{
			if (dup_error(in, STDIN_FILENO) == -1)
				return (-1);
		}
	}
	if (cmds[t_int->counter]->redir_out != -2)
	{
		if (dup_error(out, STDOUT_FILENO) == -1)
			return (-1);
	}
	return (0);
}

int	dup_close(t_cmd_op **cmds, t_ints *t_int, t_token **tokens)
{
	int		infile;
	int		outfile;

	infile = open_infile(cmds, t_int, tokens);
	outfile = open_outfile(cmds, t_int, tokens);
	if (infile == -1 || outfile == -1)
		return (-1);
	if (t_int->counter == 0)
	{
		if (dup_first(cmds, t_int, infile, outfile) == -1)
			return (-1);
	}
	else if (t_int->counter == t_int->cmd_count - 1)
	{
		if (dup_last(cmds, t_int, infile, outfile) == -1)
			return (-1);
	}
	else
		if (dup_middle(cmds, t_int, infile, outfile) == -1)
			return (-1);
	if (infile > 0)
		close(infile);
	if (outfile > 0)
		close(outfile);
	return (close_pipes(t_int), 0);
}
