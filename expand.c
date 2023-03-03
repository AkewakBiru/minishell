/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:42:49 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/03 21:41:55 by yel-touk         ###   ########.fr       */
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

int	is_exit_stat(int index, char *line, int type, int s_quote)
{
	if (line[index] == '$' && !s_quote && line[index + 1] == '?'
		&& type != delimiter && type != delimiter_q)
		return (1);
	return (0);
}

int	get_exit_stat_len(int exit_stat, int *len)
{
	char	*var_name;

	var_name = ft_itoa(t_int->e_status);
	if (!var_name)
		return (1);
	*len -= 2;
	*len += ft_strlen(var_name);
	free(var_name);
	return (0);
}

int	get_expanded_len(char *line, int type, t_list *lst, t_ints *t_int)
{
	int		i;
	int		s_quote;
	int		d_quote;
	int		len;
	char	*var_name;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	len = ft_strlen(line);
	while (line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if ((line[i] == '\'' && !d_quote) || (line[i] == '\"' && !s_quote))
			len--;
		if (is_exit_stat(i, line, type, s_quote))
			if (get_exit_stat_len(t_int->e_status, &len))
				return (-1);
		else if (line[i] == '$' && !s_quote && !d_quote && (line[i + 1] == '\"' || line[i + 1] == '\''))
			len--;
		else if (line[i] == '$' && !s_quote && type != delimiter && type != delimiter_q && (ft_isalpha(line[i + 1])
			|| (line[i + 1] == '_' && (ft_isalnum(line[i + 2]) || key_exists("_", &lst)))))
		{
			var_name = get_var_name(&line[i]);
			if (!var_name)
				return (-1);
			len -= ft_strlen(var_name) + 1;
			len += ft_strlen(get_value(var_name, lst));
			free(var_name);
		}
	}
	return (len);
}

char	*expand_line(char *line, t_token **token, int len, t_list *lst, t_ints *t_int)
{
	char	*new_line;
	int		s_quote;
	int		d_quote;
	int		i;
	int		j;
	char	*var_name;

	new_line = malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (0);
	s_quote = 0;
	d_quote = 0;
	i = -1;
	j = 0;
	while(line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if ((line[i] == '\'' && !d_quote) || (line[i] == '\"' && !s_quote))
			continue;
		if (line[i] == '$' && !s_quote && line[i + 1] == '?' && (*token)->type != delimiter && (*token)->type != delimiter_q)
		{
			var_name = ft_itoa(t_int->e_status);
			if (!var_name)
				return (0);
			ft_memcpy(&new_line[j], var_name, ft_strlen(var_name));
			i++;
			j += ft_strlen(var_name);
			free(var_name);
			continue;
		}
		if (line[i] == '$' && !s_quote && !d_quote && (line[i + 1] == '\"' || line[i + 1] == '\''))
			continue;
		if (line[i] == '$' && !s_quote && (*token)->type != delimiter && (*token)->type != delimiter_q && (ft_isalpha(line[i + 1])
			|| (line[i + 1] == '_' && ft_isalnum(line[i + 2])) || (line[i + 1] == '_' && key_exists("_", &lst))))
		{
			var_name = get_var_name(&line[i]);
			if (!var_name)
				return (0);
			ft_memcpy(&new_line[j], get_value(var_name, lst), ft_strlen(get_value(var_name, lst)));
			i += ft_strlen(var_name);
			j += ft_strlen(get_value(var_name, lst));
			if (!ft_strlen(get_value(var_name, lst)) && (*token)->type == cmd)
				(*token)->type = empty_expansion;
			free(var_name);
			continue ;
		}
		new_line[j++] = line[i];
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*expand(t_token **token, t_list *lst, t_ints *t_int)
{
	char	*new_line;
	int		len;

	len = get_expanded_len((*token)->token, (*token)->type, lst, t_int);
	if (len == -1)
		return (0);
	new_line = expand_line((*token)->token, token, len, lst, t_int);
	if (!new_line)
		return (0);
	return (new_line);
}
