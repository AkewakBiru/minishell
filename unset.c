/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:27:57 by abiru             #+#    #+#             */
/*   Updated: 2023/03/05 09:44:41 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_key(char *cmd, char *arg)
{
	int	i;
	int	flag;

	i = 0;
	flag = 1;
	if (arg + i && arg[i] == '\0')
		flag = 0;
	while (arg + i && arg[i] != '\0')
	{
		if (arg[0] && (ft_isdigit(arg[0])))
		{
			flag = 0;
			break ;
		}
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
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

int	unset_builtin(char **cmd_utils, t_list **lst, t_list **export)
{
	int	i;
	int	flag;

	i = 1;
	if (!(cmd_utils + 1) || !cmd_utils[1])
		return (0);
	flag = 0;
	while (cmd_utils + i && cmd_utils[i])
	{
		if (!check_key("unset", cmd_utils[i]))
		{
			flag = 1;
			i++;
			continue ;
		}
		if (key_exists(cmd_utils[i], lst))
			ft_list_remove_if(lst, cmd_utils[i], &ft_strcmp);
		if (key_exists(cmd_utils[i], export))
			ft_list_remove_if(export, cmd_utils[i], &ft_strcmp);
		flag = 0;
		i++;
	}
	return (flag);
}
