/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:35:02 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 09:04:02 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_sig_handler(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	exec_builtin(t_cmd_op *cmd_arr, t_list **lst, t_list **export, t_token **tokens, t_ints *t_int, char *line)
{
	char	**cmd_arg;

	cmd_arg = cmd_arr->cmd_args;
	if (!ft_strcmp(cmd_arg[0], "env"))
		print_env(lst);
	else if (!ft_strcmp(cmd_arg[0], "export"))
		export_bltin(lst, cmd_arg, export);
	else if (!ft_strcmp(cmd_arg[0], "pwd"))
		print_pwd();
	else if (!ft_strcmp(cmd_arg[0], "cd"))
	// if it has multiple args, it should go the first args directory
		chg_dir(cmd_arg, lst, export);
	else if (!ft_strcmp(cmd_arg[0], "unset"))
		unset_builtin(cmd_arg, lst, export);
	else if (!ft_strcmp(cmd_arg[0], "exit"))
		exit_shell(lst, export, cmd_arg, tokens, t_int, line);
	else if (!ft_strcmp(cmd_arg[0], "echo"))
		echo(cmd_arg);
}

void	close_pipes(int *pipes, t_ints *t_int)
{
	int	i;

	i = -1;
	while (++i < (t_int->cmd_count - 1) * 2)
		close(pipes[i]);
}

int	dup_close(t_cmd_op **cmds, t_ints *t_int, int *pipes, t_token **tokens)
{
	int		infile;
	int		outfile;
	char	*hdname;
	char	*tmp;

	infile = 0;
	outfile = 0;
	if (cmds[t_int->counter]->redir_in != -2 && cmds[t_int->counter]->redir_in != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_in]->type == redir_in)
			infile = open(tokens[cmds[t_int->counter]->redir_in + 1]->token, O_RDONLY);
		else if (tokens[cmds[t_int->counter]->redir_in]->type == here_doc)
		{
			tmp = ft_itoa(cmds[t_int->counter]->redir_in);
			hdname = ft_strjoin(".heredoc", tmp);
			infile = open(hdname, O_RDONLY);
			free(tmp);
			free(hdname);
		}
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
				dup2(pipes[1], STDOUT_FILENO);
			else
				dup2(outfile, STDOUT_FILENO);
		}
	}
	else if (t_int->counter == t_int->cmd_count - 1)
	{
		if (cmds[t_int->counter]->redir_in != -2)
		{
			if (cmds[t_int->counter]->redir_in == -1)
				dup2(pipes[2 * t_int->counter - 2], STDIN_FILENO);
			else
				dup2(infile, STDIN_FILENO);
		}
		if (cmds[t_int->counter]->redir_out != -2)
			dup2(outfile, STDOUT_FILENO);
	}
	else
	{
		if (cmds[t_int->counter]->redir_in == -1)
			dup2(pipes[2 * t_int->counter - 2], STDIN_FILENO);
		else
			dup2(infile, STDIN_FILENO);
		if (cmds[t_int->counter]->redir_out == -1)
			dup2(pipes[2 * t_int->counter + 1], STDOUT_FILENO);
		else
			dup2(outfile, STDOUT_FILENO);
	}
	if (infile > 0)
		close(infile);
	if (outfile > 0)
		close(outfile);
	close_pipes(pipes, t_int);
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

void	ex_fail_msg(char **args)
{
	DIR	*dir;

	dir = opendir(args[0]);
	if (dir)
	{
		exit_status = error_msg("Is a directory", args, 1, 126);
		closedir(dir);
	}
	else if (ft_strchr(args[0], '/')
		&& access(args[0], F_OK) != 0)
		exit_status = error_msg("No such file or directory", args, 1, 127);
	else if (access(args[0], X_OK) != 0 && access(args[0], F_OK) == 0)
		exit_status = error_msg("permission denied", args, 1, 126);
	else
		exit_status = error_msg("command not found", args, 1, 127);
}

/*
DIFFERENT ERROR MESSAGES
command not found
permission denied
no such file or directory
is a directory -> i can use opendir to check if sth is a directory or not
*/

int	dup_close_builtin(t_cmd_op **cmds, t_ints *t_int, int *pipes, t_token **tokens)
{
	int		infile;
	int		outfile;
	char	*hdname;
	char	*tmp;

	infile = 0;
	outfile = 0;
	if (cmds[t_int->counter]->redir_in != -2 && cmds[t_int->counter]->redir_in != -1)
	{
		if (tokens[cmds[t_int->counter]->redir_in]->type == redir_in)
			infile = open(tokens[cmds[t_int->counter]->redir_in + 1]->token, O_RDONLY);
		else if (tokens[cmds[t_int->counter]->redir_in]->type == here_doc)
		{
			tmp = ft_itoa(cmds[t_int->counter]->redir_in);
			hdname = ft_strjoin(".heredoc", tmp);
			infile = open(hdname, O_RDONLY);
			free(tmp);
			free(hdname);
		}
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
		else
			dup2(t_int->RLSTDIN, STDIN_FILENO);
		if (cmds[t_int->counter]->redir_out != -2)
		{
			if (cmds[t_int->counter]->redir_out == -1)
			{
				dup2(pipes[1], STDOUT_FILENO);
				// close(pipes[1]);
			}
			else
				dup2(outfile, STDOUT_FILENO);
		}
	}
	else if (t_int->counter == t_int->cmd_count - 1)
	{
		if (cmds[t_int->counter]->redir_in != -2)
		{
			if (cmds[t_int->counter]->redir_in == -1)
			{
				dup2(pipes[2 * t_int->counter - 2], STDIN_FILENO);
				close(pipes[2 * t_int->counter - 2]);
			}
			else
				dup2(infile, STDIN_FILENO);
		}
		if (cmds[t_int->counter]->redir_out != -2)
			dup2(outfile, STDOUT_FILENO);
		else
			dup2(t_int->RLSTDOUT, STDOUT_FILENO);
	}
	else
	{
		if (cmds[t_int->counter]->redir_in == -1)
		{
			dup2(pipes[2 * t_int->counter - 2], STDIN_FILENO);
			close(pipes[2 * t_int->counter - 2]);
		}
		else
			dup2(infile, STDIN_FILENO);
		if (cmds[t_int->counter]->redir_out == -1)
		{
			dup2(pipes[2 * t_int->counter + 1], STDOUT_FILENO);
			close(pipes[2 * t_int->counter + 1]);
		}
		else
			dup2(outfile, STDOUT_FILENO);
	}
	if (infile > 0)
		close(infile);
	if (outfile > 0)
		close(outfile);
	return (0);
}

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
int	exec_cmd(t_cmd_op **cmds, t_list **lst, t_list **export, char *line, t_ints *t_int, int *pipes, t_token **tokens)
{
	int		pid;
	char	**envp;

	if (!(cmds + t_int->counter) || !(cmds[t_int->counter]))
		return (0);
	if ((is_builtin(cmds[t_int->counter]->cmd) && t_int->counter == t_int->cmd_count - 1) || (!ft_strcmp(cmds[t_int->counter]->cmd, "echo")))
	{
		dup_close_builtin(cmds, t_int, pipes, tokens);
		exec_builtin(cmds[t_int->counter], lst, export, tokens, t_int, line);
	}
	else if (!is_builtin(cmds[t_int->counter]->cmd))
	{
		pid = fork();
		if (pid == -1)
		{
			perror("");
			return (-1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			close(t_int->RLSTDIN);
			close(t_int->RLSTDOUT);
			dup_close(cmds, t_int, pipes, tokens);
			envp = construct_envp(lst);
			execve(cmds[t_int->counter]->cmd, cmds[t_int->counter]->cmd_args, envp);
			ex_fail_msg(cmds[t_int->counter]->cmd_args);
			free_arr(envp);
			free_tokens(&tokens);
			free_cmd_params(cmds);
			exit(exit_status);
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

	pipes = (int *)malloc(sizeof(int) * (t_int->cmd_count - 1) * 2);
	if (!pipes)
		return (0);
	j = -1;
	while (++j < t_int->cmd_count - 1)
	{
		if (pipe(pipes + 2 * j) < 0)
		{
			perror("");
			return (0);
		}
	}
	return (pipes);
}

int	count_cmds(t_cmd_op **cmds)
{
	int	i;

	i = 0;
	while (cmds + i && cmds[i])
		i++;
	return (i);
}

int	count_non_builtin(t_cmd_op **cmds)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmds + i && cmds[i])
	{
		if (!is_builtin(cmds[i]->cmd))
			j++;
		i++;
	}
	return (j);
}

void	executor(t_cmd_op **cmds, t_list **lst, t_list **export, t_token **tokens, char *line)
{
	int		i;
	int		*pipes;
	t_ints	t_int;
	int		j;
	int		e_status;

	t_int.RLSTDIN = dup(STDIN_FILENO);
	t_int.RLSTDOUT = dup(STDOUT_FILENO);
	i = 0;
	// e_status = 0;
	t_int.counter = 0;
	t_int.cmd_count = count_cmds(cmds);
	t_int.exit_status = 0;
	pipes = 0;
	if (t_int.cmd_count > 1)
	{
		pipes = create_pipes(&t_int);
		if (!pipes)
			return ;
	}
	do_heredoc(tokens);
	while (tokens + i && tokens[i])
	{
		if (tokens[i]->type == cmd)
		{
			if (do_in_redir(tokens, i) == -1)
			{
				i++;
				continue ;
			}
			if (do_out_redir(tokens, i) == -1)
			{
				i++;
				continue ;
			}
			exec_cmd(cmds, lst, export, line, &t_int, pipes, tokens);
			t_int.counter++;
		}
		i++;
	}
	j = count_non_builtin(cmds);
	i = -1;
	if (t_int.cmd_count > 1)
		close_pipes(pipes, &t_int);
	while (++i < j)
		waitpid(-1, &e_status, 0);
	// ft_putnbr_fd(e_status, 2);
	if (WIFEXITED(e_status))
		exit_status = WEXITSTATUS(e_status);
	else if (WIFSIGNALED(e_status))
		exit_status = WTERMSIG(e_status) + 128;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	rm_hd_files(tokens);
	dup2(t_int.RLSTDIN, STDIN_FILENO);
	close(t_int.RLSTDIN);
	dup2(t_int.RLSTDOUT, STDOUT_FILENO);
	close(t_int.RLSTDOUT);
	printf("%d\n", exit_status);
}
