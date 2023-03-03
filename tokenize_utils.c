/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 16:13:29 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/03 20:30:45 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment(int *num1, int *num2)
{
	*num1 += 1;
	*num2 += 1;
}

char	*combine_strs(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

t_token	**malloc_fail(t_token **res, int i)
{
	i--;
	if (i >= 0)
	{
		free(res[i]);
		i--;
	}
	while (i >= 0)
	{
		free(res[i]->token);
		free(res[i]);
		i--;
	}
	free(res);
	return (NULL);
}

t_token	**empty_token(void)
{
	t_token	**res;

	res = malloc(sizeof(t_token *));
	if (!res)
		return (NULL);
	res[0] = NULL;
	return (res);
}
