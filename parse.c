/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 19:57:45 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_empty_expansion(t_token **token)
{
	if ((*token)->type == empty_expansion && ft_strcmp((*token)->token, "\0"))
		(*token)->type = cmd;
}

t_token	**parse(char *line, t_list *lst, t_ints *t_int)
{
	t_token	**tokens;
	char	*temp;
	int		i;

	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	if (!tokens[0])
		return (tokens);
	if (label_tokens(&tokens))
	{
		free_tokens(&tokens);
		return (NULL);
	}
	i = 0;
	while (tokens[i])
	{
		temp = tokens[i]->token;
		tokens[i]->token = expand(&tokens[i], lst, t_int);
		check_empty_expansion(&tokens[i]);
		free(temp);
		i++;
	}
	return (tokens);
}

	// i = 0;
	// while (tokens && tokens[i])
	// {
	// 	printf("token: %s, type: %u\n", tokens[i]->token, tokens[i]->type);
	// 	i++;
	// }

// t_token	**parse(char *line, t_list *lst, t_ints *t_int)
// {
// 	char	*expanded_line;
// 	t_token	**tokens;

// 	expanded_line = expand(line, lst, t_int);
// 	if (expanded_line)
// 	{
// 		tokens = tokenize(expanded_line);
// 		free(expanded_line);
// 	}
// 	else
// 		tokens = tokenize(line);
// 	if (!tokens)
// 		return (NULL);
// 	if (!tokens[0])
// 		return (tokens);
// 	if (label_tokens(&tokens))
// 	{
// 		free_tokens(&tokens);
// 		return (NULL);
// 	}
// 	int i = 0;
// 	while (tokens && tokens[i])
// 	{
// 		printf("token: %s, type: %u\n", tokens[i]->token, tokens[i]->type);
// 		i++;
// 	}
// 	return (tokens);
// }
