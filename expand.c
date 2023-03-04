/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:42:49 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 16:53:47 by youssef          ###   ########.fr       */
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

	var_name = ft_itoa(exit_stat);
	if (!var_name)
		return (1);
	ft_memcpy(new_line, var_name, ft_strlen(var_name));
	*i += 1;
	*j += ft_strlen(var_name);
	free(var_name);
	return (0);
}

int	is_quote(char c1, char c2, int s_quote, int d_quote)
{
	if (c1 == '$' && !s_quote && !d_quote && (c2 == '\"' || c2 == '\''))
		return (1);
	return (0);
}

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

int	get_var(int *i, int *j, char *line, char *new_line)
{
	char	*var_name;

	var_name = get_var_name(line);
	if (!var_name)
		return (1);
	// ft_memcpy(new_line, )
	return (0);
}

int	get_expanded_len(char *line, int type, t_list *lst, t_ints *t_int)
{
	int		i;
	int		s_quote;
	int		d_quote;
	int		len;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	len = ft_strlen(line);
	while (line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if ((line[i] == '\'' && !d_quote) || (line[i] == '\"' && !s_quote))
			len--;
		if (is_exit_stat(line[i], line[i + 1], type, s_quote))
			if (get_exit_stat_len(t_int->e_status, &len))
				return (-1);
		else if (is_quote(line[i], line[i + 1], s_quote, d_quote))
			len--;
		else if (line[i] == '$' && !s_quote
			&& is_variable(line[i + 1], line[i + 2], type, lst))
			if (get_var_len(&len, &line[i], lst))
				return (-1);
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
		if (is_exit_stat(line[i], line[i + 1], (*token)->type, s_quote))
		{
			if (get_exit_stat(&i, &j, &new_line[j], t_int->e_status))
				return (0);
			continue;
		}
		if (is_quote(line[i], line[i + 1], s_quote, d_quote))
			continue;
		if (line[i] == '$' && !s_quote
			&& is_variable(line[i + 1], line[i + 2], (*token)->type, lst))
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
