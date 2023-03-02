/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 09:12:41 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 09:47:34 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	str = 0;
	return ;
}

void	free_tokens(t_token ***tokens_p)
{
	int		i;

	i = 0;
	while ((*tokens_p)[i])
	{
		free((*tokens_p)[i]->token);
		free((*tokens_p)[i]);
		i++;
	}
	free((*tokens_p));
}

void	free_cmd_params(t_cmd_op **cmds)
{
	int	i;

	i = 0;
	while (cmds + i && cmds[i])
	{
		free(cmds[i]->cmd);
		free_arr(cmds[i]->cmd_args);
		free(cmds[i]);
		cmds[i] = 0;
		i++;
	}
	if (cmds)
		free(cmds);
	cmds = 0;
}