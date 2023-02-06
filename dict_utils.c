/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dict_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:44:46 by abiru             #+#    #+#             */
/*   Updated: 2023/02/06 20:47:44 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	free_dict(t_dict	*dict)
{
	free(dict->key);
	free(dict->value);
	free(dict);
}

int	key_exists(char *s, t_list **new)
{
	t_list	*tmp;

	tmp = *new;
	while (tmp)
	{
		if (ft_strcmp(s, ((t_dict *)tmp->content)->key) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

t_dict	*create_dict(char *key, char *value, int flag)
{
	t_dict	*new;

	new = (t_dict *)malloc(sizeof(t_dict));
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	if (flag == 1)
		new->flag = 1;
	else
		new->flag = 0;
	return (new);
}

int	update_dict(t_utils *cmd, t_list **head)
{
	t_list	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (!ft_strcmp(cmd->key, ((t_dict *)tmp->content)->key))
		{
			free(((t_dict *)tmp->content)->value);
			((t_dict *)tmp->content)->value = ft_strdup(cmd->value);
			((t_dict *)tmp->content)->flag = 1;
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}
