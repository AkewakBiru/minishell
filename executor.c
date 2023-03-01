/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:35:02 by abiru             #+#    #+#             */
/*   Updated: 2023/03/01 22:20:52 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	exec_builtin(t_cmd_op **cmd, t_list *env_pack[2], t_ints *t_int, int is_child)
{
	char	**cmd_arg;

	cmd_arg = cmd[t_int->counter]->cmd_args;
	if (!ft_strcmp(cmd_arg[0], "env"))
		print_env(env_pack + 0, t_int);
	else if (!ft_strcmp(cmd_arg[0], "export"))
		export_bltin(env_pack + 0, cmd_arg, env_pack + 1, t_int);
	else if (!ft_strcmp(cmd_arg[0], "pwd"))
		print_pwd();
	else if (!ft_strcmp(cmd_arg[0], "cd"))
	// if it has multiple args, it should go the first args directory
		chg_dir(cmd_arg, env_pack + 0, env_pack + 1, t_int);
	else if (!ft_strcmp(cmd_arg[0], "unset"))
		t_int->e_status = unset_builtin(cmd_arg, env_pack + 0, env_pack + 1);
	else if (!ft_strcmp(cmd_arg[0], "exit"))
		exit_shell(env_pack, cmd, t_int, is_child);
	else if (!ft_strcmp(cmd_arg[0], "echo"))
		t_int->e_status = echo(cmd_arg);
}

void	close_pipes(t_ints *t_int)
{
	int	i;

	i = -1;
	while (++i < (t_int->pip_count) * 2)
		close(t_int->pipes[i]);
}

int	dup_close(t_cmd_op **cmds, t_ints *t_int, t_token **tokens)
{
	int		infile;
	int		outfile;

	infile = 0;
	outfile = 0;
	if (cmds[t_int->counter]->redir_in != -2 && cmds[t_int->counter]->redir_in != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_in]->type == redir_in)
			infile = open(tokens[cmds[t_int->counter]->redir_in + 1]->token, O_RDONLY);
		else if (tokens[cmds[t_int->counter]->redir_in]->type == here_doc)
			infile = create_hd_file(cmds[t_int->counter]->redir_in, 0);
		if (infile < 0)
		{
			perror("");
			return (-1);
		}
	}
	if (cmds[t_int->counter]->redir_out != -2 && cmds[t_int->counter]->redir_out != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_out]->type == redir_out)
			outfile = open(tokens[cmds[t_int->counter]->redir_out + 1]->token, O_RDWR | O_CREAT | O_TRUNC, 0000644);
		else if (tokens[cmds[t_int->counter]->redir_out]->type == redir_out_append)
			outfile = open(tokens[cmds[t_int->counter]->redir_out + 1]->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
		if (outfile < 0)
		{
			perror("");
			return (-1);
		}
	}
	if (t_int->counter == 0)
	{
		if (cmds[t_int->counter]->redir_in != -2)
			dup2(infile, STDIN_FILENO);
		if (cmds[t_int->counter]->redir_out != -2)
		{
			if (cmds[t_int->counter]->redir_out == -1)
				dup2(t_int->pipes[1], STDOUT_FILENO);
			else
				dup2(outfile, STDOUT_FILENO);
		}
	}
	else if (t_int->counter == t_int->cmd_count - 1)
	{
		if (cmds[t_int->counter]->redir_in != -2)
		{
			if (cmds[t_int->counter]->redir_in == -1)
				dup2(t_int->pipes[2 * t_int->counter - 2], STDIN_FILENO);
			else
				dup2(infile, STDIN_FILENO);
		}
		if (cmds[t_int->counter]->redir_out != -2)
			dup2(outfile, STDOUT_FILENO);
	}
	else
	{
		if (cmds[t_int->counter]->redir_in == -1)
			dup2(t_int->pipes[2 * t_int->counter - 2], STDIN_FILENO);
		else
			dup2(infile, STDIN_FILENO);
		if (cmds[t_int->counter]->redir_out == -1)
			dup2(t_int->pipes[2 * t_int->counter + 1], STDOUT_FILENO);
		else
			dup2(outfile, STDOUT_FILENO);
	}
	if (infile > 0)
		close(infile);
	if (outfile > 0)
		close(outfile);
	close_pipes(t_int);
	return (0);
}

int	error_msg(char *msg, char **args, int num, int err)
{
	ft_putstr_fd("bash: ", 2);
	if (num == 0)
		return (1);
	if (num == 1)
		ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (err);
}

void	ex_fail_msg(t_cmd_op *cmd, char **args, t_ints *t_int)
{
	DIR	*dir;

	dir = opendir(cmd->cmd);
	if (dir)
	{
		t_int->e_status = error_msg("Is a directory", args, 1, 126);
		closedir(dir);
	}
	else if (ft_strchr(cmd->cmd, '/')
		&& access(cmd->cmd, F_OK) != 0)
		t_int->e_status = error_msg("No such file or directory", args, 1, 127);
	else if (ft_strchr(cmd->cmd, '/') && access(cmd->cmd, X_OK) != 0 && access(cmd->cmd, F_OK) == 0)
		t_int->e_status = error_msg("permission denied", args, 1, 126);
	else
		t_int->e_status = error_msg("command not found", args, 1, 127);
}

/*

if it has '/' -> and file is not found (NO such file or dir)
if file is found and is in path or has '/', but no exec permissions, permission denied
if file is found or not, if it doesn't have '/': command not found
DIFFERENT ERROR MESSAGES
command not found
permission denied
no such file or directory
is a directory -> i can use opendir to check if sth is a directory or not
*/

char *ft_join_free(t_dict *dict)
{
	char	*new;
	char	*tmp;

	tmp = ft_strdup(dict->key);
	new = ft_strjoin(tmp, "=");
	free(tmp);
	tmp = ft_strjoin(new, dict->value);
	free(new);
	return (tmp);
}

char	**construct_envp(t_list **lst)
{
	t_list	*tmp;
	int		size;
	char	**envp;
	int		i;

	tmp = *lst;
	size = ft_lstsize(tmp);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	if (!envp)
		return (0);
	while (tmp)
	{
		envp[i] = ft_join_free((t_dict *)tmp->content);
		tmp = tmp->next;
		i++;
	}
	envp[i] = 0;
	return (envp);
}

/*
	if executable is a builtin, it should be the last command to be executed
	@signal is reset to default in child process, but if the child is minishell it will be restored
	in main exec func
*/
int	exec_cmd(t_cmd_op **cmds, t_list *env_pack[2], t_ints *t_int, t_token **tokens)
{
	int		pid;
	char	**envp;

	if (!(cmds + t_int->counter) || !(cmds[t_int->counter]))
		return (0);
	if ((is_builtin(cmds[t_int->counter]->cmd) && t_int->cmd_count == 1))
	{
		dup_close(cmds, t_int, tokens);
		if (!ft_strcmp(cmds[t_int->counter]->cmd, "exit"))
			free_tokens(&tokens);
		exec_builtin(cmds, env_pack, t_int, 0);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("");
			return (-1);
		}
		t_int->child_id = pid;
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			close(t_int->RLSTDIN);
			close(t_int->RLSTDOUT);
			dup_close(cmds, t_int, tokens);
			if (is_builtin(cmds[t_int->counter]->cmd))
			{
				exec_builtin(cmds, env_pack, t_int, 1);
				exit(t_int->e_status);
			}
			envp = construct_envp(env_pack + 0);
			// printf("%s\n",cmds[t_int->counter]->cmd);
			if (ft_strchr(cmds[t_int->counter]->cmd, '/'))
				execve(cmds[t_int->counter]->cmd, cmds[t_int->counter]->cmd_args, envp);
			ex_fail_msg(cmds[t_int->counter], cmds[t_int->counter]->cmd_args, t_int);
			free_arr(envp);
			free_tokens(&tokens);
			free_cmd_params(cmds);
			ft_lstclear_dict(env_pack + 0, free);
			ft_lstclear_dict(env_pack + 1, free);
			free(t_int->pipes);
			exit(t_int->e_status);
		}
		else
			signal(SIGINT, SIG_IGN);
	}
	return (0);
}

int	*create_pipes(t_ints *t_int)
{
	int	*pipes;
	int	j;

	pipes = (int *)malloc(sizeof(int) * (t_int->pip_count) * 2);
	if (!pipes)
		return (0);
	j = -1;
	while (++j < t_int->pip_count)
	{
		if (pipe(pipes + 2 * j) < 0)
		{
			perror("");
			return (0);
		}
	}
	return (pipes);
}

int count_pipes(t_token	**tokens)
{
	int	i;
	int count;

	i = 0;
	count = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == pip)
			count++;
		i++;
	}
	return (count);
}

int count_cmd_nums(t_token	**tokens)
{
	int i = 0;
	int count = 0;

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
	dup2(t_int->RLSTDIN, STDIN_FILENO);
	close(t_int->RLSTDIN);
	dup2(t_int->RLSTDOUT, STDOUT_FILENO);
	close(t_int->RLSTDOUT);	
}

void	free_env_utils(t_strs	*cmd_list)
{
	free(cmd_list->env_p);
	cmd_list->env_p = 0;
	free_arr(cmd_list->ind_p);
	cmd_list->ind_p = 0;
	free(cmd_list);
	cmd_list = 0;
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

int check_file_existence(t_token **tokens, int i, t_ints *t_int)
{
	if (do_in_redir(tokens, i, 1, t_int) == -1)
		return (-1);
	if (do_out_redir(tokens, i, 1, t_int) == -1)
		return (-1);
	return (0);
}

int	loop_exec_cmds(t_list *env_pack[2], t_token **tokens, t_cmd_op **cmds, t_ints *t_int)
{
	int	i;

	t_int->RLSTDIN = dup(STDIN_FILENO);
	t_int->RLSTDOUT = dup(STDOUT_FILENO);
	t_int->cmd_count = count_cmd_nums(tokens);
	t_int->pip_count = count_pipes(tokens);
	t_int->counter = 0;
	t_int->pipes = 0;
	t_int->child_id = 0;
	if (t_int->pip_count > 0)
	{
		t_int->pipes = create_pipes(t_int);
		if (!t_int->pipes)
			return (1);
	}
	do_heredoc(tokens, t_int);
	redir(tokens, t_int);
	i = 0;
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == cmd)
		{
			if (check_file_existence(tokens, i, t_int) == -1 || cmds[t_int->counter]->redir_in == -5)
			{
				if (cmds[t_int->counter]->redir_in == -5)
					t_int->counter++;
				i++;
				continue ;
			}
			exec_cmd(cmds, env_pack, t_int, tokens);
			t_int->counter++;
		}
		i++;
	}
	if (t_int->cmd_count > 1)
		close_pipes(t_int);
	// printf("%d\n", t_int->e_status);
	if (t_int->cmd_count > 0 && (!(t_int->cmd_count == 1) || !(is_builtin(cmds[0]->cmd))))
		wait_for_cmds(t_int);
	reset_fd(t_int);
	rm_hd_files(tokens);
	if (t_int->pipes)
	{
		free(t_int->pipes);
		t_int->pipes = 0;
	}
	return (0);
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
	// int i = 0;
	// while (cmds[i])
	// {
	// 	printf("%s\t%d\n", cmds[i]->cmd, cmds[i]->redir_in);
	// 	i++;
	// }
	// while (tokens + i && tokens[i])
	// {
	// 	printf("%s %d\t", tokens[i]->token, tokens[i]->type);
	// 	i++;
	// }
	if (!cmds)
	{
		t_int->e_status = 1;
		return (EXIT_FAILURE);
	}
	t_int->e_status = 0;
	// only execute builtins in the parent proc if they are not in a pipeline.
	// or are simple commands
	/*
		if there is one command and is builtin, execute the command without forking and do redirection accordingly
	*/
	loop_exec_cmds(env_pack, tokens, cmds, t_int);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	free_cmd_params(cmds);
	cmds = 0;
	// printf("\n%d\n", t_int->e_status);
	return (EXIT_SUCCESS);
}
