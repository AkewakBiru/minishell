/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:57:09 by abiru             #+#    #+#             */
/*   Updated: 2023/02/06 21:05:17 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	sort_list(t_list **head)
{
	t_list	*tmp;
	t_list	*tmp2;
	char	*chr1;
	char	*chr2;
	int		flag;

	tmp = *head;
	tmp2 = *head;
	while (tmp->next)
	{
		tmp2 = tmp->next;
		while (tmp2)
		{
			if (ft_strcmp(((t_dict *)tmp->content)->key, ((t_dict *)tmp2->content)->key) > 0)
			{
				chr1 = ft_strdup(((t_dict *)tmp->content)->key);
				chr2 = ft_strdup(((t_dict *)tmp->content)->value);
				flag = ((t_dict *)tmp->content)->flag;
				free(((t_dict *)tmp->content)->key);
				free(((t_dict *)tmp->content)->value);
				((t_dict *)tmp->content)->key = ((t_dict *)tmp2->content)->key;
				((t_dict *)tmp->content)->value = ((t_dict *)tmp2->content)->value;
				((t_dict *)tmp->content)->flag = ((t_dict *)tmp2->content)->flag;
				((t_dict *)tmp2->content)->key = chr1;
				((t_dict *)tmp2->content)->value = chr2;
				((t_dict *)tmp2->content)->flag = flag;
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return ;
}
