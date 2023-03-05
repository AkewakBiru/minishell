/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:49:53 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:12:05 by abiru            ###   ########.fr       */
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

int	check_key_name(char *cmd, char *arg)
{
	int	i;
	int	flag;

	i = 0;
	flag = 1;
	if (arg + i && arg[i] == '\0')
		flag = 0;
	while (arg + i && arg[i])
	{
		if (arg[0] && (ft_isdigit(arg[0])
				|| arg[0] == '='))
		{
			flag = 0;
			break ;
		}
		if (arg && !check_alphanumeric(arg))
		{
			flag = 0;
			break ;
		}
		i++;
	}
	if (flag == 0)
		custom_err_msg(cmd, arg);
	return (flag);
}

int	export_error(char *cmd, char *arg, t_ints *t_int, int *i)
{
	if (!ft_strcmp(arg, "_") || !ft_strncmp(arg, "_=", 2))
	{
		(*i)++;
		return (1);
	}
	if (!check_key_name(cmd, arg))
	{
		t_int->e_status = 1;
		(*i)++;
		return (1);
	}
	return (0);
}

void	export_bltin(t_list **lst, char **cmd_utils, t_list **export,
		t_ints *t_int)
{
	int		i;
	t_dict	*dict;

	i = 1;
	if (!(cmd_utils + i) || !cmd_utils[i])
		return (print_list(export, t_int));
	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
		return ;
	while (cmd_utils + i && cmd_utils[i])
	{
		if (export_error("export", cmd_utils[i], t_int, &i))
			continue ;
		create_key_val(&dict, cmd_utils[i]);
		if (ft_strchr(cmd_utils[i], '='))
			update_env(dict, lst);
		update_env(dict, export);
		t_int->e_status = 0;
		free_dict(dict, 1);
		i++;
	}
	free(dict);
}
