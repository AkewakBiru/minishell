/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 20:51:41 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 22:01:08 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd_nums(t_token	**tokens)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == cmd)
			count++;
		i++;
	}
	return (count);
}

void	reset_fd(t_ints *t_int)
{
	dup2(t_int->rlstdin, STDIN_FILENO);
	close(t_int->rlstdin);
	dup2(t_int->rlstdout, STDOUT_FILENO);
	close(t_int->rlstdout);
}

int	wait_for_cmds(t_ints *t_int)
{
	int	e_status;

	(void)e_status;
	if (waitpid(t_int->child_id, &e_status, 0) == -1)
		return (0);
	if (WIFEXITED(e_status))
		t_int->e_status = WEXITSTATUS(e_status);
	else if (WIFSIGNALED(e_status))
		t_int->e_status = WTERMSIG(e_status) + 128;
	while (waitpid(-1, &e_status, 0) >= 0)
		;
	return (t_int->e_status);
}

int	check_file_existence(t_token **tokens, int i, t_ints *t_int)
{
	if (do_in_redir(tokens, i, 1, t_int) == -1)
		return (-1);
	if (do_out_redir(tokens, i, 1, t_int) == -1)
		return (-1);
	return (0);
}

int	init_utils(t_token **tokens, t_ints *t_int)
{
	t_int->rlstdin = dup(STDIN_FILENO);
	t_int->rlstdout = dup(STDOUT_FILENO);
	t_int->cmd_count = count_cmd_nums(tokens);
	t_int->pip_count = count_pipes(tokens);
	t_int->counter = 0;
	t_int->pipes = 0;
	t_int->child_id = 0;
	if (t_int->pip_count > 0)
	{
		t_int->pipes = create_pipes(t_int);
		if (!t_int->pipes)
			return (-1);
	}
	return (0);
}
