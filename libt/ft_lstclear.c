/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:05:15 by abiru             #+#    #+#             */
/*   Updated: 2022/10/02 13:42:29 by abiru            ###   ########.fr       */
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
