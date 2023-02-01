/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 11:13:15 by abiru             #+#    #+#             */
/*   Updated: 2023/02/01 15:50:56 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(t_dict *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->dict = (t_dict *)malloc(sizeof(t_dict));
	if (!new_node->dict)
	{
		free(new_node);
		return (0);
	}
	new_node->dict->key = ft_strdup(content->key);
	new_node->dict->value = ft_strdup(content->value);
	new_node->next = 0;
	return (new_node);
}
