
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
	while(line[i] && line[i] != ' ' && line[i] != '$' && line[i] != '<'
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
		if (line[i] == '$' && !s_quote
			&& ((line[i + 1] >= 'A' && line[i + 1] <= 'Z')
			|| (line[i + 1] >= 'a' && line[i + 1] <= 'z')
			|| (line[i + 1] == '_' && line[i + 2] != '\0' && line[i + 2] != ' '
			&& line[i + 2] != '\'' &&line[i + 2] != '\"')))
			return (1);
	}
	return (0);
}

int	get_expanded_len(char *line, t_list *lst, int i)
{
	int		s_quote;
	int		d_quote;
	int		len;
	char	*var_name;
	char	*value;

	s_quote = 0;
	d_quote = 0;
	len = ft_strlen(line);
	while(line[++i])
	{
		check_quotes(line[i], &s_quote, &d_quote);
		if (line[i] == '$' && !s_quote
			&& ((line[i + 1] >= 'A' && line[i + 1] <= 'Z')
			|| (line[i + 1] >= 'a' && line[i + 1] <= 'z')
			|| (line[i + 1] == '_' && line[i + 2] != '\0' && line[i + 2] != ' '
			&& line[i + 2] != '\'' &&line[i + 2] != '\"')))
		{
			var_name = get_var_name(&line[i]);
			len -= ft_strlen(var_name) + 1;
			value = get_value(var_name, lst);
			len += ft_strlen(value);
			free(var_name);
			free(value);
		}
	}
	return (len);
}

char	*expand_line(char *line, int len)
{
	char *new_line;

	// new_line = malloc(sizeof(char) * (len + 1));
	return (new_line);
}

char	*expand(char *line, t_list *lst)
{
	char	*new_line;
	int		len;

	// if (!should_expand(line))
	// 	return (line);
	len = get_expanded_len(line, lst, -1);
	if (!len)
		return (0);
	new_line = expand_line(line, len);
	free(line);
	return (0);
}

int main()
{
	// char *s = "echo \'hi\'f\'\" \"\"\'\"\'\"\'there\'  ";
	char *s = "echo e$ffs";
	// char *s = "\"\"echo - | > \"a\" \"hi\"";//"he\'\"\"\'e\' cho hi\'|hi >>there";
	// char *s = " echo hi \"\" there\"     s\"\"\'$x\"";
	// char *s = "ec\"\"\'\'ho";
	printf("%s\n", s);
	// printf("should_expand: %d\n", should_expand(s));
	// get_expanded_len(s, -1);
	// printf("name: %s\n\n", get_var_name(s));
}