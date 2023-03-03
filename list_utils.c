/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:57:09 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 09:06:28 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

static void	ft_swap(t_dict *dict, t_dict *dict2)
{
	char	*chr1;
	char	*chr2;
	int		flag;

	chr1 = ft_strdup(dict->key);
	chr2 = ft_strdup(dict->value);
	flag = dict->flag;
	free(dict->key);
	free(dict->value);
	dict->key = dict2->key;
	dict->value = dict2->value;
	dict->flag = dict2->flag;
	dict2->key = chr1;
	dict2->value = chr2;
	dict2->flag = flag;
}

void	sort_list(t_list **head)
{
	t_list	*tmp;
	t_list	*tmp2;

	tmp = *head;
	tmp2 = *head;
	while (tmp->next)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (ft_strcmp(((t_dict *)tmp->content)->key,
					((t_dict *)tmp2->content)->key) > 0)
				ft_swap((t_dict *)tmp->content, (t_dict *)tmp2->content);
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return ;
}

void	ft_list_remove_if(t_list **head, char *str, int (*cmp)())
{
	t_list	*cur;

	if (head == NULL || *head == NULL)
		return ;
	cur = *head;
	if (!(*cmp)(((t_dict *)cur->content)->key, str))
	{
		*head = cur->next;
		del_node(cur);
		return ;
	}
	cur = *head;
	ft_list_remove_if(&cur->next, str, cmp);
}
