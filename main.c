/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:38:21 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_node(void *node)
{
	free(((t_dict *)((t_list *)node)->content)->key);
	free(((t_dict *)((t_list *)node)->content)->value);
	free(((t_dict *)((t_list *)node)->content));
	free(node);
	return ;
}

void	handle_signal(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	init_env_sig(t_list *env_pack[2], char **envp, t_ints *t_int)
{
	if (signal(SIGINT, handle_signal) != SIG_ERR)
		t_int->e_status = 1;
	signal(SIGQUIT, SIG_IGN);
	env_pack[0] = 0;
	env_pack[1] = 0;
	create_env(env_pack + 0, envp);
	create_env(env_pack + 1, envp);
	update_shell(env_pack);
}

int	start(t_list *env_pack[2], t_ints *t_int)
{
	char	*line;
	t_token	**tokens;

	line = readline("Yash-10.0$ ");
	if (!line)
	{
		ft_lstclear_dict(env_pack + 0, free);
		ft_lstclear_dict(env_pack + 1, free);
		t_int->e_status = 1;
		return (ft_putendl_fd("exit", 1), -1);
	}
	if (!ft_strlen(line))
		return (free(line), 0);
	add_history(line);
	tokens = parse(line, env_pack[0], t_int);
	free(line);
	if (!tokens)
	{
		printf("Syntax error!\n");
		t_int->e_status = 258;
		return (0);
	}
	executor(env_pack, tokens, t_int);
	return (free_tokens(&tokens), 0);
}

int	main(int ac, char **av, char **envp)
{
	t_list	*env_pack[2];
	t_ints	t_int;

	if (ac != 1)
		return (1);
	init_env_sig(env_pack, envp, &t_int);
	t_int.e_status = 0;
	while (1)
	{
		if (start(env_pack, &t_int) == -1)
			return (t_int.e_status);
	}
	return ((void)av, t_int.e_status);
}
