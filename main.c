/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/01/31 17:00:25 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	print_env(t_list **lst)
{
	t_list	*tmp = *lst;
	while (tmp)
	{
		printf("%s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

void	export_bltin(t_list **lst)
{
	(void)lst;
	printf("in export function\n");
}

int	is_builtin(const char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void	exec_builtin(const char *cmd, t_list **lst)
{
	if (ft_strcmp(cmd, "env") == 0)
		print_env(lst);
	else if (ft_strcmp(cmd, "export") == 0)
		export_bltin(lst);
}

void	exec_pipex(t_list **lst)
{
	(void)lst;
	printf("pipex func\n");
}

void	exec_cmd(const char *cmd, t_list **lst)
{
	if (is_builtin(cmd) == 1)
		exec_builtin(cmd, lst);
	else
		exec_pipex(lst);
}

void	create_env(t_list **head, char **envp)
{
	int		i;
	t_list	*new;

	i = 0;
	while (envp[i])
	{
		new = ft_lstnew(envp[i]);
		ft_lstadd_back(head, new);
		i++;
	}
}

int main(int ac, char **av, char **envp)
{
	t_list	*lst;
	(void)av;
	if (ac != 1)
		return (1);

	lst = NULL;
	create_env(&lst, envp);
	while (1)
	{
		char *line = readline("Minishell> ");
		exec_cmd(line, &lst);
	}
	printf("here\n");	
}
