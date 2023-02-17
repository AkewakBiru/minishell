/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:50:47 by abiru             #+#    #+#             */
/*   Updated: 2023/02/17 22:51:56 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

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

int exit_shell(t_list **lst, t_list **export, t_utils *cmd_utils, t_token **tokens, char *line)
{
	(void)tokens;
	int status = cmd_utils->EXIT_STATUS;
	ft_lstclear_dict(lst, free);
	ft_lstclear_dict(export, free);
	if (cmd_utils->key)
		free(cmd_utils->key);
	if (cmd_utils->value)
		free(cmd_utils->value);
	free_split(cmd_utils->cmd_arg);
	free(line);
	// ft_lstclear(tokens, free);
	exit(status);
}