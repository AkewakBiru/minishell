/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 20:43:31 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 10:12:14 by abiru            ###   ########.fr       */
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

void	exec_builtin(t_cmd_op **cmd, t_list *env_pack[2], t_ints *t_int,
		int is_child)
{
	char	**cmd_arg;

	cmd_arg = cmd[t_int->counter]->cmd_args;
	if (!ft_strcmp(cmd_arg[0], "env"))
		print_env(env_pack + 0, t_int);
	else if (!ft_strcmp(cmd_arg[0], "export"))
		export_bltin(env_pack + 0, cmd_arg, env_pack + 1, t_int);
	else if (!ft_strcmp(cmd_arg[0], "pwd"))
		print_pwd(cmd_arg);
	else if (!ft_strcmp(cmd_arg[0], "cd"))
		chg_dir(cmd_arg, env_pack + 0, env_pack + 1, t_int);
	else if (!ft_strcmp(cmd_arg[0], "unset"))
		t_int->e_status = unset_builtin(cmd_arg, env_pack + 0, env_pack + 1);
	else if (!ft_strcmp(cmd_arg[0], "exit"))
		exit_shell(env_pack, cmd, t_int, is_child);
	else if (!ft_strcmp(cmd_arg[0], "echo"))
		t_int->e_status = echo(cmd_arg);
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
