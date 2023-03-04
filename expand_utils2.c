/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 19:41:40 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 22:02:32 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_variable(char c1, char c2, int type, t_list *lst)
{
	if (type != delimiter && type != delimiter_q && (ft_isalpha(c1)
			|| (c1 == '_' && (ft_isalnum(c2) || key_exists("_", &lst)))))
		return (1);
	return (0);
}

int	get_var_len(int *len, char *line, t_list *lst)
{
	char	*var_name;

	var_name = get_var_name(line);
	if (!var_name)
		return (1);
	*len -= ft_strlen(var_name) + 1;
	*len += ft_strlen(get_value(var_name, lst));
	free(var_name);
	return (0);
}

int	get_var(t_expand *t, t_token **token, char *new_line, t_list *lst)
{
	char	*var_name;

	var_name = get_var_name(&t->l[t->i]);
	if (!var_name)
		return (1);
	ft_memcpy(new_line, get_value(var_name, lst),
		ft_strlen(get_value(var_name, lst)));
	t->i += ft_strlen(var_name);
	t->j += ft_strlen(get_value(var_name, lst));
	if (!ft_strlen(get_value(var_name, lst)) && (*token)->type == cmd)
		(*token)->type = empty_expansion;
	free(var_name);
	return (0);
}

int	if_var(t_expand	*t_e, t_token **token, t_list *lst, char *nl)
{
	int		type;
	char	*line;

	type = (*token)->type;
	line = (*token)->token;
	if (line[t_e->i] == '$' && !t_e->sq
		&& line[t_e->i + 1] && is_variable(line[t_e->i + 1], line[t_e->i + 2], type, lst))
	{
		if (get_var(t_e, token, &nl[t_e->j], lst))
			return (1);
		return (2);
	}
	return (0);
}
