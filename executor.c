/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:35:02 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 13:34:05 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
if it has '/' -> and file is not found (NO such file or dir)
if file is found and is in path or has '/', but no exec permissions, 
permission denied
if file is found or not, if it doesn't have '/': command not found
DIFFERENT ERROR MESSAGES
command not found
permission denied
no such file or directory
is a directory -> i can use opendir to check if sth is a directory or not
*/

/*
	if executable is a builtin, it should be the last command to be executed
	@signal is reset to default in child process, but if the child is minishell 
	it will be restored in main exec func
*/

int	child_exec(t_cmd_op **cmds, t_list *env_pack[2], t_ints *t_int,
	t_token **tokens)
{
	char	**envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(t_int->rlstdin);
	close(t_int->rlstdout);
	dup_close(cmds, t_int, tokens);
	if (is_builtin(cmds[t_int->counter]->cmd))
	{
		if (!ft_strcmp(cmds[t_int->counter]->cmd, "exit")
			&& count_arg(cmds[t_int->counter]->cmd_args) <= 2)
			free_tokens(&tokens);
		exec_builtin(cmds, env_pack, t_int, 1);
		free_mem_child(cmds, env_pack, t_int, tokens);
		exit(t_int->e_status);
	}
	envp = construct_envp(env_pack + 0);
	if (ft_strchr(cmds[t_int->counter]->cmd, '/'))
		execve(cmds[t_int->counter]->cmd, cmds[t_int->counter]->cmd_args, envp);
	ex_fail_msg(cmds[t_int->counter], cmds[t_int->counter]->cmd_args, t_int);
	free_arr(envp);
	free_mem_child(cmds, env_pack, t_int, tokens);
	exit(t_int->e_status);
}

int	exec_cmd(t_cmd_op **cmds, t_list *env_pack[2], t_ints *t_int,
	t_token **tokens)
{
	int		pid;

	if (!(cmds + t_int->counter) || !(cmds[t_int->counter]))
		return (0);
	if ((is_builtin(cmds[t_int->counter]->cmd) && t_int->cmd_count == 1))
	{
		dup_close(cmds, t_int, tokens);
		if (!ft_strcmp(cmds[t_int->counter]->cmd, "exit")
			&& count_arg(cmds[t_int->counter]->cmd_args) <= 2)
			free_tokens(&tokens);
		exec_builtin(cmds, env_pack, t_int, 0);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			return (perror(""), -1);
		t_int->child_id = pid;
		if (pid == 0)
			child_exec(cmds, env_pack, t_int, tokens);
		else
			signal(SIGINT, SIG_IGN);
	}
	return (0);
}

void	finish_exec(t_token **tokens, t_cmd_op **cmds, t_ints *t_int)
{
	if (t_int->cmd_count > 1)
		close_pipes(t_int);
	if (t_int->cmd_count > 0 && (!(t_int->cmd_count == 1)
			|| !(is_builtin(cmds[0]->cmd))))
		wait_for_cmds(t_int);
	reset_fd(t_int);
	rm_hd_files(tokens);
	if (t_int->pipes)
	{
		free(t_int->pipes);
		t_int->pipes = 0;
	}
}

	/*
		inside the while loop while a pipe is not encountered
		do redirection.
		check the output of the redirection and if it is fine
		and check if there is a command. If there is:
		execute the command and go to the next pipe and
		continue
	*/
// void	look_for_cmd(t_token **tokens, t_ints *t_int, int i)

int	loop_exec_cmds(t_list *env_pack[2], t_token **tokens,
					t_cmd_op **cmds, t_ints *t_int)
{
	int	i;

	if (init_utils(tokens, t_int) == -1)
		return (1);
	do_heredoc(tokens, env_pack, t_int);
	i = 0;
	while (tokens + i && tokens[i])
	{
		if (do_redirection(tokens, t_int, i) != -5)
		{	
			if (find_cmd(tokens, i) >= 0)
			{
				cmds[t_int->counter]->redir_in = find_stdin(tokens,
						find_cmd(tokens, i));
				cmds[t_int->counter]->redir_out = find_stdout(tokens,
						find_cmd(tokens, i));
				exec_cmd(cmds, env_pack, t_int, tokens);
				t_int->counter++;
			}
		}
		else if (find_cmd(tokens, i) >= 0)
			update_status_counter(t_int);
		i = go_to_next_pip(tokens, i);
	}
	return (finish_exec(tokens, cmds, t_int), 0);
}

int	executor(t_list *env_pack[2], t_token **tokens, t_ints *t_int)
{
	t_strs		*cmd_list;
	t_cmd_op	**cmds;

	cmd_list = init_struct();
	cmd_list->env_p = ft_strdup(get_val(env_pack + 0, "PATH"));
	cmd_list->ind_p = ft_split(cmd_list->env_p, ':');
	cmd_list->cmd_len = count_cmd_nums(tokens);
	cmds = create_cmd_list(&cmd_list, tokens);
	free_env_utils(cmd_list);
	if (!cmds)
	{
		t_int->e_status = 1;
		return (EXIT_FAILURE);
	}
	t_int->e_status = 0;
	loop_exec_cmds(env_pack, tokens, cmds, t_int);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	free_cmd_params(cmds);
	cmds = 0;
	return (EXIT_SUCCESS);
}
