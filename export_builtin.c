/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:49:53 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 10:11:40 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_dict *cmd, t_list **env)
{
	t_dict	*new;
	t_list	*node;
	char	*key;
	char	*value;

	key = ft_strdup(cmd->key);
	value = ft_strdup(cmd->value);
	if (key_exists(key, env) == 1)
	{
		if (cmd->flag == 1)
			update_dict(key, value, env);
		return (free(key));
	}
	new = create_dict(key, value, cmd->flag);
	if (!new)
		return (ft_lstclear(env, free));
	node = ft_lstnew_dict(new);
	free(new);
	free(key);
	free(value);
	node->next = 0;
	ft_lstadd_back(env, node);
}

int	check_alphanumeric(char *cmd)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = ft_split(cmd, '=');
	if (!tmp || !tmp[0])
		return (1);
	while (tmp[0] + i && tmp[0][i])
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
		{
			free_split(tmp);
			return (0);
		}
		i++;
	}
	free_split(tmp);
	return (1);
}

int	check_key_names(char *cmd, char **cmd_utils)
{
	int	i;
	int	flag;

	i = 0;
	flag = 1;
	while (cmd_utils + i && cmd_utils[i])
	{
		flag = 1;
		if (cmd_utils[i] + 0 && cmd_utils[i][0] == '\0')
			flag = 0;
		if (cmd_utils[i] + 0 && (ft_isdigit(cmd_utils[i][0])
			|| cmd_utils[i][0] == '='))
			flag = 0;
		if (cmd_utils[i] + 0 && !check_alphanumeric(cmd_utils[i]))
			flag = 0;
		if (flag == 0)
			custom_err_msg(cmd, cmd_utils[i]);
		i++;
	}
	return (flag);
}

void	create_key_val(t_dict **dict, char *cmd_utils)
{
	t_dict	*tmp;

	tmp = *dict;
	if (ft_strchr(cmd_utils, '='))
		tmp->flag = 1;
	else
		tmp->flag = 0;
	tmp->key = ft_strndup(cmd_utils, '=');
	if (ft_strchr(cmd_utils, '='))
		tmp->value = ft_strdup(ft_strchr(cmd_utils, '=') + 1);
	else
		tmp->value = ft_strdup("");
}

void	export_bltin(t_list **lst, char **cmd_utils, t_list **export,
		t_ints *t_int)
{
	int		i;
	t_dict	*dict;

	i = 1;
	if (!(cmd_utils + i) || !cmd_utils[i])
		return (print_list(export, t_int));
	if (!check_key_names(cmd_utils[0], cmd_utils))
	{
		t_int->e_status = 1;
		return ;
	}
	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
		return ;
	while (cmd_utils + i && cmd_utils[i])
	{
		create_key_val(&dict, cmd_utils[i]);
		if (ft_strchr(cmd_utils[i], '='))
			update_env(dict, lst);
		update_env(dict, export);
		free_dict(dict, 1);
		i++;
	}
}
