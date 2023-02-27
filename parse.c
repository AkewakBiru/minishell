/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 12:04:45 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/27 14:19:08 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	**empty_token()
{
	t_token	**res;

	res = malloc(sizeof(t_token *));
	if (!res)
		return (NULL);
	res[0] = NULL;
	return (res);
}

t_token	**parse(char *line, t_list *lst)
{
	char	*expanded_line;
	t_token	**tokens;

	expanded_line = expand(line, lst);
	if (expanded_line)
	{
		if (!ft_strcmp(expanded_line, "\0"))
		{
			tokens = empty_token();
			free(expanded_line);
			return (tokens);
		}
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