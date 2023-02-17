/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:05:15 by abiru             #+#    #+#             */
/*   Updated: 2023/02/17 22:20:50 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*current;

	if (!lst || !del)
		return ;
	temp = *lst;
	current = *lst;
	while (current != NULL)
	{
		current = current -> next;
		ft_lstdelone(temp, del);
		temp = current;
	}
	*lst = NULL;
	return ;
}

void	ft_lstclear_dict(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*current;

	if (!lst || !del)
		return ;
	temp = *lst;
	current = *lst;
	while (current != NULL)
	{
		current = current -> next;
		free(((t_dict *)temp->content)->key);
		free(((t_dict *)temp->content)->value);
		ft_lstdelone(temp, del);
		temp = current;
	}
	*lst = NULL;
	return ;
}
