/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 20:28:26 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/03 20:30:55 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_one_token(int *j, int *num, const char *s)
{
	int		d_quote;
	int		s_quote;

	s_quote = 0;
	d_quote = 0;
	while (s[*j] && (s_quote || d_quote
			|| (!is_white_space(s[*j]) && s[*j] != '|' && !is_redir(s[*j]))))
	{
		check_quotes(s[*j], &s_quote, &d_quote);
		*j += 1;
		*num += 1;
	}
}

void	read_until_seperator(int *index, int *num, char const *s, t_token **res)
{
	*num = 0;
	(*res)->type = unset;
	while (s[*index] && is_white_space(s[*index]))
		*index += 1;
	while (s[*index] && !is_white_space(s[*index]) && s[*index] != '|'
		&& !is_redir(s[*index]) && s[*index] != '\'' && s[*index] != '\"')
		increment(index, num);
}

void	label_special(int *index, int *num, char const *s, t_token **res)
{
	if (s[*index] == '|')
	{
		increment(index, num);
		(*res)->type = pip;
	}
	else
	{
		while (is_redir(s[*index]))
			increment(index, num);
		(*res)->type = redirection;
	}
}
