/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:35:02 by abiru             #+#    #+#             */
/*   Updated: 2023/02/26 21:35:03 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_left_pipe(t_token **tokens, int i)
{
	int j;

	j = i - 1;
	while (j > 0 && tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		j--;
	}
	return (j);
}

int	do_in_redir(t_token **tokens, int i)
{
	int	j;
	int	k;

	if (i > 0)
		j = get_left_pipe(tokens, i);
	else
		j = 0;
	if (tokens + j && tokens[j] && (tokens[j]->type == pip || tokens[j]->type == cmd))
		j++;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens + j && tokens[j] && tokens[j]->type == redir_in)
		{
			k = open(tokens[j + 1]->token, O_RDONLY);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
}

int	do_out_redir(t_token **tokens, int i)
{
	int	j;
	int	k;
	if (i > 0)
		j = get_left_pipe(tokens, i);
	else
		j = 0;
	if (tokens + j && tokens[j] && (tokens[j]->type == pip || tokens[j]->type == cmd))
		j++;
	while (tokens + j && tokens[j] && tokens[j]->type != pip)
	{
		if (tokens + j && tokens[j] && tokens[j]->type == redir_out)
		{
			k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_TRUNC, 0000644);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		else if (tokens + j && tokens[j] && tokens[j]->type == redir_out_append)
		{
			k = open(tokens[j + 1]->token, O_RDWR | O_CREAT | O_APPEND, 0000644);
			if (k < 0)
			{
				perror("");
				return (-1);
			}
			close(k);
		}
		j++;
	}
	return (0);
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

int	error_msg(char *msg, char **args, int num)
{
	write(2, msg, ft_strlen(msg));
	if (num == 0)
		return (1);
	write(2, ": ", 2);
	if (num == 1)
		write(2, args[0], ft_strlen(args[0]));
	return (write(2, "\n", 1));
}

void	ex_fail_msg(char **args)
{
	if (ft_strchr(args[0], '/')
		&& access(args[0], F_OK) != 0)
		error_msg("No such file or directory", args, 1);
	else
		error_msg("command not found", args, 1);
}

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
		if (cmds[t_int->counter]->redir_out != -2)
		{
			if (cmds[t_int->counter]->redir_out == -1)
			{
				dup2(pipes[1], STDOUT_FILENO);
				close(pipes[1]);
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

int	exec_cmd(t_cmd_op **cmds, t_list **lst, t_list **export, char *line, t_ints *t_int, int *pipes, t_token **tokens)
{
	int		pid;
	char	**envp;

	if (!(cmds + t_int->counter) || !(cmds[t_int->counter]))
		return (0);
	if (is_builtin(cmds[t_int->counter]->cmd))
	{
		dup_close_builtin(cmds, t_int, pipes, tokens);
		exec_builtin(cmds[t_int->counter], lst, export, tokens, t_int, line);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			close(t_int->RLSTDIN);
			close(t_int->RLSTDOUT);
			dup_close(cmds, t_int, pipes, tokens);
			envp = construct_envp(lst);
			execve(cmds[t_int->counter]->cmd, cmds[t_int->counter]->cmd_args, envp);
			ex_fail_msg(cmds[t_int->counter]->cmd_args);
			free_arr(envp);
			free_tokens(&tokens);
			free_cmd_params(cmds);
			exit(1);
		}
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

	t_int.RLSTDIN = dup(STDIN_FILENO);
	t_int.RLSTDOUT = dup(STDOUT_FILENO);
	i = 0;
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
				continue ;
			if (do_out_redir(tokens, i) == -1)
				continue ;
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
		waitpid(-1, &(t_int.exit_status), 0);
	if (WIFEXITED(t_int.exit_status))
		t_int.exit_status = WEXITSTATUS(t_int.exit_status);
	else if (WIFSIGNALED(t_int.exit_status))
		t_int.exit_status = WTERMSIG(t_int.exit_status) + 128;
	rm_hd_files(tokens);
	dup2(t_int.RLSTDIN, STDIN_FILENO);
	close(t_int.RLSTDIN);
	dup2(t_int.RLSTDOUT, STDOUT_FILENO);
	close(t_int.RLSTDOUT);
	printf("%d\n", t_int.exit_status);
}
