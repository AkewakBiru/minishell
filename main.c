/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 23:58:26 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_node(void *node)
{
	free(((t_dict *)((t_list *)node)->content)->key);
	free(((t_dict *)((t_list *)node)->content)->value);
	free(((t_dict *)((t_list *)node)->content));
	return ;
}

/*
* Added "*" before tokens because of compilation error
*/

void	handle_signal(int sig)
{
	(void)sig;
	// means we have moved into a newline
	rl_on_new_line();
	// redisplay the prompt (after the changes are made)
	rl_redisplay();
	write(1, "  \n", 3);
	// add whatever text is given as first argument to the char* buffer, in this case empty string
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	check_line(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ' || line[i] != '\t' || line[i] != '\n'
			|| line[i] != '\v' || line[i] != '\f' || line[i] != '\r')
			return (1);
	}
	return (0);
}

void	free_tokens(t_token ***tokens_p)
{
	int		i;

	i = 0;
	while ((*tokens_p)[i])
	{
		free((*tokens_p)[i]->token);
		free((*tokens_p)[i]);
		i++;
	}
	free((*tokens_p));
}

void	free_cmd_params(t_cmd_op **cmds)
{
	int	i;
	
	i = 0;
	while (cmds + i && cmds[i])
	{
		free(cmds[i]->cmd);
		free_arr(cmds[i]->cmd_args);
		free(cmds[i]);
		cmds[i] = 0;
		i++;
	}
	if (cmds)
		free(cmds);
	cmds = 0;
}

int	update_shell(t_list *envp[2])
{
	unsigned long long	n;
	t_dict	*dict;

	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
	{
		perror("malloc");
		return (1);
	}
	dict->key = ft_strdup("SHLVL");
	dict->flag = 1;
	if (key_exists("SHLVL", &(envp)[0]))
	{
		if (get_val(&(envp)[0], "SHLVL"))
		{
			n = ft_atoi(get_val(&(envp)[0], "SHLVL"));
			if (n >= 1000)
				n = 0;
			n++;
		}
		else
			n = 1;
		dict->value = ft_itoa(n);
		update_env(dict, &(envp)[0]);
		update_env(dict, &(envp)[1]);
		free_dict(dict);
		return (0);
	}
	n = 1;
	dict->value = ft_itoa(n);
	update_env(dict, &(envp)[0]);
	update_env(dict, &(envp)[1]);
	free_dict(dict);
	return (0);
}

void	init_env_sig(t_list *env_pack[2], char **envp)
{
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	env_pack[0] = 0;
	env_pack[1] = 0;
	create_env(env_pack + 0, envp);
	create_env(env_pack + 1, envp);
	update_shell(env_pack);
}

int	main(int ac, char **av, char **envp)
{
	t_token	**tokens;
	char	*line;
	t_list	*env_pack[2];
	t_ints	t_int;

	(void)av;
	if (ac != 1)
		return (1);
	init_env_sig(env_pack, envp);
	t_int.e_status = 0;
	while (1)
	{
		line = readline("Yash-10.0$ ");
		if (!line)
		{
			ft_lstclear_dict(env_pack + 0, free);
			ft_lstclear_dict(env_pack + 1, free);
			ft_putendl_fd("exit", 2);
			return (0);
		}
		if (!ft_strlen(line))
		{
			free(line);
			continue;
		}
		add_history(line);
		tokens = parse(line, env_pack[0], &t_int);
		if (!tokens)
		{
			printf("Syntax error!\n");
			free(line);
			t_int.e_status = 258;
			continue;
		}
		free(line);
		executor(env_pack, tokens, &t_int);
		free_tokens(&tokens);
	}
	return (t_int.e_status);
}
