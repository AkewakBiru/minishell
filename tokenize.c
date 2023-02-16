/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:31:18 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/15 22:29:17 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(char c)
{
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

void	check_dquotes(char c, int s_quote, int *d_quote)
{
	if (c == '\"' && !s_quote && !*d_quote)
		*d_quote = 1; //return(1);
	else if (c == '\"' && !s_quote)
		*d_quote = 0; //return (0);
	// return (d_quote);
}

void	check_squotes(char c, int *s_quote, int d_quote)
{
	if (c == '\'' && !d_quote && !*s_quote)
		*s_quote = 1;//return(1);
	else if (c == '\'' && !d_quote)
		*s_quote = 0; //return(0);
	//return (s_quote);
}

static int	get_num_tokens(char const *s, char c)
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
		return (0);
	while (s[i] == c && c != 0)
		i++;
	if (s[i] == 0)
		return (0);
	while (s[i] != 0)
	{
		check_squotes(s[i], &s_quote, d_quote);
		check_dquotes(s[i], s_quote, &d_quote);
		if (((s[i] == c || s[i] == '|' || (is_redir(s[i]) && !is_redir(s[i + 1])))
			&& s[i + 1] != 0 && s[i + 1] != c && !d_quote && !s_quote)
			|| (((is_redir(s[i + 1]) && !is_redir(s[i])) || s[i + 1] == '|') && !d_quote && !s_quote))
			num++;
		// printf("c: %c, sq: %d, dq: %d, num: %d\n", s[i], s_quote, d_quote, num);
		i++;
	}
	if (s_quote || d_quote)
		return (0);
	return (num);
}

char	**malloc_fail(char **res, int i)
{
	i--;
	while (i >= 0)
	{
		free(res[i]);
		i--;
	}
	free(res);
	return (0);
}

// int	skip_start(int j, char const *s, int *s_quote, int *d_quote)
// {
// 	while (s[j] && ((s[j] == ' ' && !*s_quote && !*d_quote)
// 			|| (s[j] == '\"' && !*s_quote) || (s[j] == '\'' && !*d_quote)))
// 		{
// 			printf("letter: %c, sq: %d, dq: %d\n", s[j], *s_quote, *d_quote);
// 			check_squotes(s[j], s_quote, *d_quote);
// 			check_dquotes(s[j], *s_quote, d_quote);
// 			j++;
// 		}
// 	return (j);
// }

char	*combine_strs(int j, int num, const char *s, int opened_q)
{
	printf("combine_str called\n");
	(void) j;
	(void) num;
	(void) s;
	(void) opened_q;
	// char	*read;
	// int	d_quote;
	// int	s_quote;

	// read = ft_substr(s, j - num, num);
	// while (s[j] && s[j] != ' ')
	// {
	// 	while (s[j] && (opened_q || d_quote || s_quote))
	// 	{
	// 		//read until quote closed
	// 		j++;
	// 	}
	// 	opened_q = 0;
	// 	while (s[j] && s[j] != ' ' && s[j] != '\'' && s[j] != '\"')
	// 	{
	// 		// "keine ahnung";
	// 	}
	// }
	return NULL;
}

char	**split_tokens(char const *s, char c, char ***res)
{
	int	i;
	int	j;
	int	num;
	int	d_quote;
	int	s_quote;

	i = 0;
	j = 0;
	d_quote = 0;
	s_quote = 0;
	while (i < get_num_tokens(s, c))
	{
		num = 0;
		//skip start
		// j = skip_start(j, s, &s_quote, &d_quote);
		while (s[j] && (s[j] == c //&& !s_quote && !d_quote)
			|| (s[j] == '\"' && !s_quote) || (s[j] == '\'' && !d_quote)))
		{
			printf("letter: %c, sq: %d, dq: %d\n", s[j], s_quote, d_quote);
			check_squotes(s[j], &s_quote, d_quote);
			check_dquotes(s[j], s_quote, &d_quote);
			j++;
		}
		printf("entering while-loop 2\n");
		while (s[j] && (s_quote || d_quote || (s[j] != c && s[j] != '|' && !is_redir(s[j]))))
		{
			check_squotes(s[j], &s_quote, d_quote);
			check_dquotes(s[j], s_quote, &d_quote);
			printf("letter: %c, sq: %d, dq: %d\n", s[j], s_quote, d_quote);
			if ((s[j] == '\'' && !d_quote) || (s[j] == '\"' && !s_quote))
				break;
			j++;
			num++;
		}
		if (!num && s[j] == '|')
		{
			num++;
			j++;
		}
		if (!num && is_redir(s[j]))
		{
			while (is_redir(s[j]))
			{
				num++;
				j++;
			}
		}
		printf("left while loop with letter %c\n", s[j]);
		printf("j: %d, num:%d\n", j, num);
		if ((s[j] == '\'' && !d_quote) && s[j + 1] != c)
			(*res)[i] = combine_strs(j, num, s, s_quote);
		else if ((s[j] == '\"' && !s_quote) && s[j + 1] != c)
			(*res)[i] = combine_strs(j, num, s, d_quote);
		else
			(*res)[i] = ft_substr(s, j - num, num);
		printf("word: %s\n", (*res)[i]);
		if ((*res)[i] == NULL)
			return (malloc_fail(*res, i));
		if (s[j] == c || s[j] == '\'' || s[j] == '\"')
			j++;
		i++;
	}
	(*res)[i] = 0;
	return (*res);
}

char	**tokenize(char const *line)
{
	char	**res;
	int		count;

	count = get_num_tokens(line, ' ');
	if (!count)
		return (NULL);
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	return (split_tokens(line, ' ', &res));
}

int main()
{
	// char *s = "echo \'hi\'f\'\" \"\"\'\"\'\"\'there\'  ";
	// char *s = "\" \"\'\'\"gr\"ep||echo>h>>< | \'| hi>|\'there  ";
	char *s = "e\'cho hi\'|hi >>>there";//"\"\"echo>\"\" \"hi\"";
	// char *s = "echo hi \"\" there\"     s\"\"\'$x\"";
	printf("%s\n", s);
	printf("%d\n\n", get_num_tokens(s, ' '));
	char **r = tokenize(s);
	int i = 0;
	while (r + i && r[i])
	{
		printf("%s\n", r[i]);
		i++;
	}
	printf("i: %d\n", i);
}
