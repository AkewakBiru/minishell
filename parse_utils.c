/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:42:42 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 19:56:58 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_white_space(char c)
{
	if (c == ' ' || (c >= 9 && c < 13))
		return (1);
	return (0);
}

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
