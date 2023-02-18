/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/18 13:18:06 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	tokenize2(char *line, t_list **head)
// {
// 	int		i;
// 	t_list	*new_token;
// 	t_token	*token;
// 	char	**tokens;

// 	tokens = ft_split(line, ' ');
// 	i = 0;
// 	while (tokens[i])
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->token = tokens[i];
// 		new_token = ft_lstnew(token);
// 		ft_lstadd_back(head, new_token);
// 		i++;
// 	}
// }

// int	label_tokens(t_token ***tokens)
// {
// 	int	i;

// 	i = 0;
// 	if ((*tokens)[i] && (*tokens)[i]->type != unset)
// 		return (1);
// 	(*tokens)[i++]->type = cmd;
// 	while ((*tokens)[i])
// 	{
// 		if ()
// 	}
// }

t_token	**parse(char *line)
{
	(void) line;
	t_token	**tokens;

	tokens = tokenize(line);
	//need to free tokens eventually
	// tokenize2(line, tokens);
	return (tokens);
}

// int main(int argc, char **argv)
// {
// 	t_list 
// }