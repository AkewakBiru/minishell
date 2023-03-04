/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:42:49 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 19:47:30 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *line)
{
	int		i;
	char	*var_name;

	i = 1;
	while (ft_isalnum(line[i]) || line[i] == '_')
		i++;
	var_name = malloc(sizeof(char) * (i));
	if (!var_name)
		return (0);
	var_name[--i] = '\0';
	while (--i >= 0)
		var_name[i] = line[i + 1];
	return (var_name);
}

char	*get_value(char *var_name, t_list *lst)
{
	while (lst)
	{
		if (!ft_strcmp(var_name, ((t_dict *)lst->content)->key))
			return (((t_dict *)lst->content)->value);
		lst = lst->next;
	}
	return (0);
}

int	get_expanded_len(char *line, int type, t_list *lst, t_ints *t_int)
{
	int		i;
	int		s_quote;
	int		d_quote;
	int		len;

	init_quotes_index(&i, 0, &s_quote, &d_quote);
	len = ft_strlen(line);
	while (line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if ((line[i] == '\'' && !d_quote) || (line[i] == '\"' && !s_quote))
			len--;
		if (is_exit_stat(line[i], line[i + 1], type, s_quote))
		{
			if (get_exit_stat_len(t_int->e_status, &len))
				return (-1);
		}
		else if (is_quote(line[i], line[i + 1], s_quote, d_quote))
			len--;
		else if (line[i] == '$' && !s_quote
			&& is_variable(line[i + 1], line[i + 2], type, lst))
			if (get_var_len(&len, &line[i], lst))
				return (-1);
	}
	return (len);
}

char	*expand_line(t_token **token, char *nl, t_list *lst, t_ints *t_int)
{
	t_expand	t;

	init_quotes_index(&t.i, &t.j, &t.sq, &t.dq);
	t.l = (*token)->token;
	while (t.l[++t.i])
	{
		check_quotes(t.l[t.i], &t.sq, &t.dq);
		if ((t.l[t.i] == '\'' && !t.dq) || (t.l[t.i] == '\"' && !t.sq))
			continue ;
		if (is_exit_stat(t.l[t.i], t.l[t.i + 1], (*token)->type, t.sq))
		{
			if (get_exit_stat(&t.i, &t.j, &(nl[t.j]), t_int->e_status))
				return (0);
			continue ;
		}
		if (is_quote(t.l[t.i], t.l[t.i + 1], t.sq, t.dq))
			continue ;
		t.flag = if_var(&t, token, lst, nl);
		if (t.flag == 1)
			return (0);
		if (t.flag == 0)
			(nl)[t.j++] = t.l[t.i];
	}
	(nl)[t.j] = '\0';
	return (nl);
}

char	*expand(t_token **token, t_list *lst, t_ints *t_int)
{
	char	*new_line;
	int		len;

	len = get_expanded_len((*token)->token, (*token)->type, lst, t_int);
	if (len == -1)
		return (0);
	new_line = malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (0);
	new_line = expand_line(token, new_line, lst, t_int);
	if (!new_line)
		return (0);
	return (new_line);
}
