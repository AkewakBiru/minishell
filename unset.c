/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:27:57 by abiru             #+#    #+#             */
/*   Updated: 2023/02/11 18:28:18 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	unset_builtin(t_utils *cmd_utils, t_list **lst, t_list **export)
{
	if (!cmd_utils->cmd_arg[1])
		return ;
	if (key_exists(cmd_utils->cmd_arg[1], lst))
		ft_list_remove_if(lst, cmd_utils->cmd_arg[1], &ft_strcmp);
	if (key_exists(cmd_utils->cmd_arg[1], export))
		ft_list_remove_if(export, cmd_utils->cmd_arg[1], &ft_strcmp);
	return ;
}
