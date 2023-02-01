/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 14:17:22 by abiru             #+#    #+#             */
/*   Updated: 2023/02/01 15:42:10 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*temp;

	temp = NULL;
	new = NULL;
	if (!f || !del || !lst)
		return (NULL);
	while (lst != NULL)
	{
		temp = ft_lstnew((*f)(lst -> dict));
		if (!temp)
		{
			ft_lstclear(&new, del);
			return (0);
		}
		ft_lstadd_back(&new, temp);
		lst = lst -> next;
	}
	return (new);
}
