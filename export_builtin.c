/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:49:53 by abiru             #+#    #+#             */
/*   Updated: 2023/02/17 22:19:08 by abiru            ###   ########.fr       */
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
		printf("%s=%s\n", ((t_dict *)tmp->content)->key,
			((t_dict *)tmp->content)->value);
		tmp = tmp->next;
	}
}

void	print_list(t_list **lst)
{
	t_list	*tmp;

	sort_list(lst);
	tmp = *lst;
	while (tmp)
	{
		if (((t_dict *)tmp->content)->flag == 1)
			printf("declare -x %s=\"%s\"\n",
				((t_dict *)tmp->content)->key, ((t_dict *)tmp->content)->value);
		else
			printf("declare -x %s\n", ((t_dict *)tmp->content)->key);
		tmp = tmp->next;
	}
}

void	update_env(t_utils *cmd, t_list **env)
{
	t_dict	*new;
	t_list	*node;

	char *key = ft_strdup(cmd->key);
	char *value = ft_strdup(cmd->value);
	if (key_exists(key, env) == 1)
	{
		if (cmd->flag == 1)
			update_dict(key, value, env);
		// key already present so should be freed
		free(key);
		return ;
	}
	new = create_dict(key, value, cmd->flag);
	if (!new)
	{
		ft_lstclear(env, free);
		return ;
	}
	node = ft_lstnew_dict(new);
	free(new);
	free(key);
	free(value);
	node->next = 0;
	ft_lstadd_back(env, node);
	// printf("INSIDE update end");
}

void	export_bltin(t_list **lst, t_utils *cmd_utils, t_list **export)
{
	if (!cmd_utils->cmd_arg || !cmd_utils->cmd_arg[1])
		print_list(export);
	else
	{
		if (cmd_utils->flag)
			update_env(cmd_utils, lst);
		update_env(cmd_utils, export);
	}
}

void	create_env(t_list **head, char **envp)
{
	int		i;
	t_list	*new;
	t_dict	*dict;

	i = -1;
	while (envp[++i])
	{
		dict = create_dict(ft_strndup(envp[i], '='),
				ft_strdup(ft_strchr(envp[i], '=') + 1), 1);
		new = ft_lstnew_dict(dict);
		ft_lstadd_back(head, new);
		free_dict(dict);
	}
}
