/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/02/01 21:33:39 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	print_env(t_list **lst)
{
	t_list	*tmp;

	tmp = *lst;
	while (tmp)
	{
		printf("%s=%s\n", tmp->dict->key, tmp->dict->value);
		tmp = tmp->next;
	}
}

int	check_existence(char *s, t_list **new)
{
	t_list	*tmp;

	tmp = *new;
	while (tmp)
	{
		if (ft_strcmp(s, tmp->dict->key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

t_list	*find_biggest_key(t_list **lst)
{
	t_list	*tmp;
	t_list	*biggest;

	tmp = *lst;
	biggest = tmp;
	while (tmp)
	{
		if (ft_strcmp(biggest->dict->key, tmp->dict->key) < 0)
			biggest = tmp;
		tmp = tmp->next;
	}
	return (biggest);
}

t_dict	*find_smallest_key(t_list **lst, t_list **new)
{
	t_list	*tmp;
	t_list	*smallest;

	tmp = *lst;
	smallest = find_biggest_key(lst);
	while (tmp)
	{
		if (ft_strcmp(smallest->dict->key, tmp->dict->key) > 0 && !check_existence(tmp->dict->key, new))
			smallest = tmp;
		tmp = tmp->next;
	}
	return (smallest->dict);
}

t_list	**sort_list(t_list **new, t_list **lst)
{
	t_list	*tmp;
	t_list	*dup_node;

	tmp = *lst;
	while (tmp)
	{
		dup_node = ft_lstnew(find_smallest_key(lst, new));
		ft_lstadd_back(new, dup_node);
		tmp = tmp->next;
	}
	return (0);
}

void	print_list(t_list	**lst)
{
	t_list	*tmp;

	tmp = *lst;
	while (tmp)
	{
		printf("declare -x %s=%s\n", tmp->dict->key, tmp->dict->value);
		tmp = tmp->next;
	}
}

void	export_bltin(t_list **lst, char **args)
{
	t_list	*dup;

	dup = 0;
	sort_list(&dup, lst);
	if (!args || args[1] == 0)
		print_list(&dup);
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

void	exec_builtin(const char *cmd, char **args, t_list **lst)
{
	if (ft_strcmp(cmd, "env") == 0)
		print_env(lst);
	else if (ft_strcmp(cmd, "export") == 0)
		export_bltin(lst, args);
}

void	exec_pipex(t_list **lst)
{
	(void)lst;
	printf("pipex func\n");
}

void	exec_cmd(const char *cmd, t_list **lst)
{
	char	**cmd_list;

	cmd_list = ft_split(cmd, ' ');
	if (is_builtin(cmd) == 1)
		exec_builtin(cmd, cmd_list, lst);
	else
		exec_pipex(lst);
}

void	free_exit(t_list **head)
{
	(void)head;
	exit(1);
}

void	free_dict(t_dict	*dict)
{
	free(dict->key);
	free(dict->value);
	free(dict);
}

void	create_env(t_list **head, char **envp)
{
	int		i;
	t_list	*new;
	t_dict	*dict;

	i = 0;
	dict = NULL;
	while (envp[i])
	{
		dict = (t_dict *)malloc(sizeof(t_dict));
		if (!dict)
			free_exit(head);
		dict->key = ft_strndup(envp[i], '=');
		dict->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		new = ft_lstnew(dict);
		// printf("%s=%s\n", new->dict->key, new->dict->value);
		ft_lstadd_back(head, new);
		free_dict(dict);
		i++;
	}
}

int main(int ac, char **av, char **envp)
{
	t_list	*lst;

	lst = NULL;
	(void)av;
	if (ac != 1)
		return (1);
	create_env(&lst, envp);
	while (1)
	{
		char *line = readline("Minishell> ");
		exec_cmd(line, &lst);
	}
}
