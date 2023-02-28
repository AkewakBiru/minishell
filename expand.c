/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:42:49 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/28 15:19:39 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

// int	is_special(char c)
// {
// 	if (c == '|' || c == '>' || c == '<' || c == ';' || c == '\\')
// 		return (1);
// 	return (0);
// }

char	*get_var_name(char *line)
{
	int		i;
	char	*var_name;

	i = 1;
	while(line[i] && !is_white_space(line[i]) && line[i] != '$' && line[i] != '<'
			&& line[i] != '>' && line[i] != '|' && line[i] != '\'' && line[i] != '\"')
		i++;
	var_name = malloc(sizeof(char) * (i));
	if (!var_name)
		return (0);
	var_name[--i] = '\0';
	while (--i >= 0)
		var_name[i] = line[i + 1];
	return(var_name);
}

char	*get_value(char *var_name, t_list *lst)
{
	while(lst)
	{
		if (!ft_strncmp(var_name, ((t_dict *)lst->content)->key, ft_strlen(var_name)))
			return (((t_dict *)lst->content)->value);
		lst = lst->next;
	}
	return (0);
}

int	should_expand(char *line)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = -1;
	s_quote = 0;
	d_quote = 0;
	while(line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if (line[i] == '$' && !s_quote && ((line[i + 1] == '?')
			|| (!d_quote && (line[i + 1] == '\"' || line[i + 1] == '\''))
			|| (line[i + 1] >= 'A' && line[i + 1] <= 'Z')
			|| (line[i + 1] >= 'a' && line[i + 1] <= 'z')
			|| (line[i + 1] == '_' && line[i + 2] != '\0' && !is_white_space(line[i + 2])
			&& line[i + 2] != '\'' &&line[i + 2] != '\"')))
			return (1);
	}
	return (0);
}

int	get_expanded_len(char *line, t_list *lst)
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
	while(line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if (line[i] == '$' && !s_quote && line[i + 1] == '?')
			len--;
		else if (line[i] == '$' && !s_quote && !d_quote && (line[i + 1] == '\"' || line[i + 1] == '\''))
			len--;
		else if (line[i] == '$' && !s_quote
			&& ((line[i + 1] >= 'A' && line[i + 1] <= 'Z')
			|| (line[i + 1] >= 'a' && line[i + 1] <= 'z')
			|| (line[i + 1] == '_' && line[i + 2] != '\0' && !is_white_space(line[i + 2])
			&& line[i + 2] != '\'' &&line[i + 2] != '\"')))
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

char	*expand_line(char *line, int len, t_list *lst)
{
	char *new_line;
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
		if (line[i] == '$' && !s_quote && line[i + 1] == '?')
		{
			new_line[j++] = exit_status + 48;
			i++;
			continue;
		}
		if (line[i] == '$' && !s_quote && !d_quote && (line[i + 1] == '\"' || line[i + 1] == '\''))
			continue;
		if (line[i] == '$' && !s_quote
			&& ((line[i + 1] >= 'A' && line[i + 1] <= 'Z')
			|| (line[i + 1] >= 'a' && line[i + 1] <= 'z')
			|| (line[i + 1] == '_' && line[i + 2] != '\0' && !is_white_space(line[i + 2])
			&& line[i + 2] != '\'' &&line[i + 2] != '\"')))
		{
			var_name = get_var_name(&line[i]);
			if (!var_name)
				return (0);
			ft_memcpy(&new_line[j], get_value(var_name, lst), ft_strlen(get_value(var_name, lst)));
			i += ft_strlen(var_name);
			j += ft_strlen(get_value(var_name, lst));
			free(var_name);
			continue;
		}
		new_line[j++] = line[i];
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*expand(char *line, t_list *lst)
{
	char	*new_line;
	int		len;

	if (!should_expand(line))
		return (0);
	len = get_expanded_len(line, lst);
	if (len == -1)
		return (0);
	new_line = expand_line(line, len, lst);
	if (!new_line)
		return (0);
	return (new_line);
}

// int main()
// {
// 	// char *s = "echo \'hi\'f\'\" \"\"\'\"\'\"\'there\'  ";
// 	char *s = "echo e$ffs";
// 	// char *s = "\"\"echo - | > \"a\" \"hi\"";//"he\'\"\"\'e\' cho hi\'|hi >>there";
// 	// char *s = " echo hi \"\" there\"     s\"\"\'$x\"";
// 	// char *s = "ec\"\"\'\'ho";
// 	printf("%s\n", s);
// 	// printf("should_expand: %d\n", should_expand(s));
// 	// get_expanded_len(s, -1);
// 	// printf("name: %s\n\n", get_var_name(s));
// }