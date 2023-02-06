/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 11:13:15 by abiru             #+#    #+#             */
/*   Updated: 2023/02/06 20:33:03 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof (t_list));
	if (node == 0)
		return (node);
	node->content = content;
	node->next = NULL;
	return (node);
}

t_list	*ft_lstnew_dict(t_dict *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = (t_dict *)malloc(sizeof(t_dict));
	if (!new_node->content)
	{
		free(new_node);
		return (0);
	}
	((t_dict *)new_node->content)->key = ft_strdup(content->key);
	((t_dict *)new_node->content)->value = ft_strdup(content->value);
	((t_dict *)new_node->content)->flag = content->flag;
	new_node->next = 0;
	return (new_node);
}
