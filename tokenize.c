/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:31:18 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/03 20:31:01 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_token(char const *s, int index, int s_quote, int d_quote)
{
	if (((is_white_space(s[index]) || s[index] == '|'
				|| (is_redir(s[index]) && !is_redir(s[index + 1])))
			&& s[index + 1] != 0 && !is_white_space(s[index + 1])
			&& !d_quote && !s_quote) || (((is_redir(s[index + 1])
					&& !is_redir(s[index])) || s[index + 1] == '|')
			&& !d_quote && !s_quote))
		return (1);
	return (0);
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
		if (is_token(s, i, s_quote, d_quote))
			num++;
		i++;
	}
	if (s_quote || d_quote)
		return (-1);
	return (num);
}

int	handle_quotes(int *j, int num, const char *s, t_token **token)
{
	char	*res;

	res = ft_substr(s, *j - num, num);
	if (!res)
		return (0);
	while (s[*j] && !is_white_space(s[*j]) && s[*j] != '|' && !is_redir(s[*j]))
	{
		num = 0;
		read_one_token(j, &num, s);
		if (num)
		{
			res = combine_strs(res, ft_substr(s, *j - num, num));
			if (!res)
				return (0);
			continue ;
		}
		*j += 1;
	}
	(*token)->token = res;
	return (*j);
}

t_token	**split_tokens(char const *s, t_token ***res)
{
	int	i;
	int	j;
	int	num;

	i = -1;
	j = 0;
	while (++i < get_num_tokens(s))
	{
		read_until_seperator(&j, &num, s, &(*res)[i]);
		if (s[j] == '\'' || s[j] == '\"')
		{
			if (!handle_quotes(&j, num, s, (&(*res)[i])))
				return (malloc_fail(*res, i));
			if (i > 0 && (*res)[i - 1] && (*res)[i - 1]->type == redirection)
				(*res)[i]->type = delimiter_q;
			continue ;
		}
		if (!num && (s[j] == '|' || is_redir(s[j])))
			label_special(&j, &num, s, &((*res)[i]));
		(*res)[i]->token = ft_substr(s, j - num, num);
		if ((*res)[i]->token == NULL)
			return (malloc_fail(*res, i));
	}
	(*res)[i] = NULL;
	return (*res);
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
