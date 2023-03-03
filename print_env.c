/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 10:11:43 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 10:12:41 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_list **lst, t_ints *t_int)
{
	t_list	*tmp;

	tmp = *lst;
	while (tmp)
	{
		printf("%s=%s\n", ((t_dict *)tmp->content)->key,
			((t_dict *)tmp->content)->value);
		tmp = tmp->next;
	}
	t_int->e_status = 0;
}

void	print_list(t_list **lst, t_ints *t_int)
{
	t_list	*tmp;

	if (!lst || !(*lst))
		return ;
	sort_list(lst);
	tmp = *lst;
	while (tmp)
	{
		if (((t_dict *)tmp->content)->flag == 1)
			printf("declare -x %s=\"%s\"\n",
				((t_dict *)tmp->content)->key, ((t_dict *)tmp->content)->value);
		else
			printf("declare -x %s\n", ((t_dict *)tmp->content)->key);
		tmp = tmp->next;
	}
	t_int->e_status = 0;
}
