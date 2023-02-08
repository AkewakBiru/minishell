/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/08 21:43:51 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**tokenize2(char *line)
{
	int	i;
	int	open_quote;

	i = 0;
	open_quote = 0;
	(void)line;
	(void)i;
	(void)open_quote;
	return (0);
}

void	tokenize(t_list **head, char *line)
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
	tokenize(tokens, line);
}

// int main(int argc, char **argv)
// {
// 	t_list 
// }