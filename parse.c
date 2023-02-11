/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/12 00:44:50 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand(char ***tokens)
{
	(void) tokens;
}

void	tokenize2(char *line, t_list **head)
{
	int		i;
	t_list	*new_token;
	t_token	*token;
	char	**tokens;

	tokens = ft_split(line, ' ');
	i = 0;
	while (tokens[i])
	{
		token = malloc(sizeof(t_token));
		token->token = tokens[i];
		new_token = ft_lstnew(token);
		ft_lstadd_back(head, new_token);
		i++;
	}
}

void	parse(char *line, t_list **tokens)
{
	char	**tokens_arr;

	tokens_arr = tokenize(line);
	expand(&tokens_arr);
	//need to free tokens eventually
	tokenize2(line, tokens);
}

// int main(int argc, char **argv)
// {
// 	t_list 
// }