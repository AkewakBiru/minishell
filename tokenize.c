/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:31:18 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/01 15:49:21 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	check_quotes(char c, int *s_quote, int *d_quote)
{
	if (c == '\"' && !*s_quote && !*d_quote)
		*d_quote = 1;
	else if (c == '\"' && !*s_quote)
		*d_quote = 0;
	if (c == '\'' && !*d_quote && !*s_quote)
		*s_quote = 1;
	else if (c == '\'' && !*d_quote)
		*s_quote = 0;
}

static int	get_num_tokens(char const *s)
{
	int	i;
	int	num;
	int	d_quote;
	int	s_quote;

	num = 1;
	i = 0;
	d_quote = 0;
	s_quote = 0;
	if (!s)
		return (-1);
	while (is_white_space(s[i]))
		i++;
	if (s[i] == 0)
		return (0);
	while (s[i] != 0)
	{
		check_quotes(s[i], &s_quote, &d_quote);
		if (((is_white_space(s[i]) || s[i] == '|' || (is_redir(s[i]) && !is_redir(s[i + 1])))
			&& s[i + 1] != 0 && !is_white_space(s[i + 1]) && !d_quote && !s_quote)
			|| (((is_redir(s[i + 1]) && !is_redir(s[i])) || s[i + 1] == '|') && !d_quote && !s_quote))
			num++;
		// printf("c: %c, sq: %d, dq: %d, num: %d\n", s[i], s_quote, d_quote, num);
		i++;
	}
	if (s_quote || d_quote)
		return (-1);
	return (num);
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

char	*combine_strs(char *s1, char *s2)
{
	char *res;
	
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

int	handle_quotes(int j, int num, const char *s, t_token **token)
{
	int		d_quote;
	int		s_quote;
	char	*res;

	s_quote = 0;
	d_quote = 0;
	res = ft_substr(s, j - num, num);
	if (!res)
		return (0);
	while (s[j] && !is_white_space(s[j]) && s[j] != '|' && !is_redir(s[j]))
	{
		num = 0;
		while (s[j] && s[j] != '\'' && s[j] != '\"' && !is_white_space(s[j]) && s[j] != '|' && !is_redir(s[j]))
		{
			num++;
			j++;
		}
		if (num)
		{
			res = combine_strs(res, ft_substr(s, j - num, num));
			if (!res)
				return (0);
			continue;
		}
		if (s[j] == '\'' || s[j] == '\"')
			check_quotes(s[j++], &s_quote, &d_quote);
		while (s[j] && ((s_quote && s[j] != '\'') || (d_quote && s[j] != '\"')))
		{
			check_quotes(s[j++], &s_quote, &d_quote);
			num++;
		}
		check_quotes(s[j], &s_quote, &d_quote);
		res = combine_strs(res, ft_substr(s, j - num, num));
		if (!res)
			return (0);
		j++;
	}
	(*token)->token = res;
	return (j);
}

t_token	**split_tokens(char const *s, t_token ***res)
{
	int	i;
	int	j;
	int	num;
	int	d_quote;
	int	s_quote;

	i = 0;
	j = 0;
	(void)d_quote;
	(void)s_quote;
	d_quote = 0;
	s_quote = 0;
	while (i < get_num_tokens(s))
	{
		num = 0;
		(*res)[i]->type = unset;
		//skip start
		while (s[j] && is_white_space(s[j]))
			j++;
		// printf("entering while-loop 2\n");
		while (s[j] && !is_white_space(s[j]) && s[j] != '|' && !is_redir(s[j]) && s[j] != '\'' && s[j] != '\"')
		{
			// printf("letter: %c, sq: %d, dq: %d\n", s[j], s_quote, d_quote);
			j++;
			num++;
		}
		// printf("left while loop with letter %c\n", s[j]);
		if (s[j] == '\'' || s[j] == '\"')
		{
			// printf("combine called\n");
			j = handle_quotes(j, num, s, (&(*res)[i]));
			if (!j)
				return (malloc_fail(*res, i));
			if ((*res)[i - 1] && (*res)[i - 1]->type == redirection)
				(*res)[i]->type = delimiter_q;
			i++;
			continue;
		}
		if (!num && s[j] == '|')
		{
			num++;
			j++;
			(*res)[i]->type = pip;
		}
		if (!num && is_redir(s[j]))
		{
			while (is_redir(s[j]))
			{
				num++;
				j++;
			}
			(*res)[i]->type = redirection;
		}
		(*res)[i]->token = ft_substr(s, j - num, num);
		if ((*res)[i]->token == NULL)
			return (malloc_fail(*res, i));
		// printf("word: %s\n", (*res)[i]->token);
		i++;
	}
	(*res)[i] = NULL;
	return (*res);
}

t_token	**empty_token()
{
	t_token	**res;

	res = malloc(sizeof(t_token *));
	if (!res)
		return (NULL);
	res[0] = NULL;
	return (res);
}

t_token	**tokenize(char const *line)
{
	t_token	**res;
	int		count;
	int		i;

	count = get_num_tokens(line);
	if (count == -1)
		return (NULL);
	if (!count)
		return (empty_token());
	res = malloc(sizeof(t_token *) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < count)
	{
		res[i] = malloc(sizeof(t_token));
		if (!res[i])
			return (malloc_fail(res, i));
		i++;
	}
	split_tokens(line, &res);
	return (res);
}

// int main()
// {
// 	// char *s = "echo \'hi\'f\'\" \"\"\'\"\'\"\'there\'  ";
// 	char *s = "\" \"\'\'\"gr\"ep||echo>h>>< | \'| hi>|\'there  ";
// 	// char *s = "he\'\"\"\'e\' cho hi\'|hi >>>there";//"\"\"echo>\"\" \"hi\"";
// 	// char *s = " echo hi \"\" there\"     s\"\"\'$x\"";
// 	// char *s = "ec\"\"\'\'ho";
// 	printf("%s\n", s);
// 	printf("%d\n\n", get_num_tokens(s));
// 	t_token **r = tokenize(s);
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
