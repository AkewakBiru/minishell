/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:27:57 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 18:03:14 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_builtin(char **cmd_utils, t_list **lst, t_list **export)
{
	int	i;

	i = 1;
	if (!(cmd_utils + 1) || !cmd_utils[1])
		return (0);
	if (!check_key_names(cmd_utils[0], cmd_utils))
		return (1);
	while (cmd_utils + i && cmd_utils[i])
	{
		if (key_exists(cmd_utils[i], lst))
			ft_list_remove_if(lst, cmd_utils[i], &ft_strcmp);
		if (key_exists(cmd_utils[i], export))
			ft_list_remove_if(export, cmd_utils[i], &ft_strcmp);
		i++;
	}
	return (0);
}
