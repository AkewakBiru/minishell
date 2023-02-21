/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/21 13:00:42 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	**parse(char *line, t_list *lst)
{
	char	*expanded_line;
	t_token	**tokens;

	expanded_line = expand(line, lst);
	// printf("new_line %s\n", expanded_line);
	if (expanded_line)
	{
		tokens = tokenize(expanded_line);
		free(expanded_line);
	}
	else
		tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	if (label_tokens(&tokens))
	{
		free_tokens(&tokens);
		return (NULL);
	}
	// int i = 0;
	// while (tokens && tokens[i])
	// {
	// 	printf("token: %s, type: %u\n", tokens[i]->token, tokens[i]->type);
	// 	i++;
	// }
	//need to free tokens eventually
	// tokenize2(line, tokens);
	return (tokens);
}

// int main()
// {
// 	char *s = "echo \'hi\'f\'\" \"\"\'\"\'\"\'there\'  ";
// 	// char *s = "\" \"\'\'\"gr\"ep||echo>h>>< | \'| hi>|\'there  ";
// 	// char *s = "\"\"echo - | > \"a\" \"hi\"";//"he\'\"\"\'e\' cho hi\'|hi >>there";
// 	// char *s = " echo hi \"\" there\"     s\"\"\'$x\"";
// 	// char *s = "ec\"\"\'\'ho";
// 	printf("%s\n", s);
// 	// printf("%d\n\n", get_num_tokens(s));
// 	// t_token **r = tokenize(s);
// 	t_token **r = parse(s);
// 	int i = 0;
// 	while (r && r[i])
// 	{
// 		printf("token: %s, type: %u\n", r[i]->token, r[i]->type);
// 		free(r[i]->token);
// 		free(r[i]);
// 		i++;
// 	}
// 	printf("i: %d\n", i);
// 	free(r);
// }