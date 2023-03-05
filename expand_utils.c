/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 19:40:11 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/05 16:51:51 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_quotes_index(int *i, int *j, int *s_quote, int *d_quote)
{
	*i = -1;
	if (j)
		*j = 0;
	*s_quote = 0;
	*d_quote = 0;
}

int	is_quote(char c1, char c2, int s_quote, int d_quote)
{
	if (c1 == '$' && !s_quote && !d_quote && (c2 == '\"' || c2 == '\''))
		return (1);
	return (0);
}

int	is_exit_stat(char c1, char c2, int type, int s_quote)
{
	if (c1 == '$' && !s_quote && c2 == '?'
		&& type != delimiter && type != delimiter_q)
		return (1);
	return (0);
}

int	get_exit_stat_len(int exit_stat, int *len)
{
	char	*var_name;

	var_name = ft_itoa(exit_stat);
	if (!var_name)
		return (1);
	*len -= 2;
	*len += ft_strlen(var_name);
	free(var_name);
	return (0);
}

int	get_exit_stat(int *i, int *j, char *new_line, int exit_stat)
{
	char	*var_name;

	(void) exit_stat;
	var_name = ft_itoa(g_e_stat);
	if (!var_name)
		return (1);
	ft_memcpy(new_line, var_name, ft_strlen(var_name));
	*i += 1;
	*j += ft_strlen(var_name);
	free(var_name);
	return (0);
}
