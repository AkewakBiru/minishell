/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youssef <youssef@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:31:18 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/11 16:22:27 by youssef          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//should split spaces but not sapces inside doubele or single quotes

int	check_dquotes(char c, int s_quote, int d_quote)
{
	if (c == '\"' && !s_quote && !d_quote)
		return(1);
	else if (c == '\"' && !s_quote)
		return (0);
	return (d_quote);
}

int	check_squotes(char c, int s_quote, int d_quote)
{
	if (c == '\'' && !d_quote && !s_quote)
		return(1);
	else if (c == '\'' && !d_quote)
		return(0);
	return (s_quote);
}

static int	get_number_str(char const *s, char c)
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
		s_quote = check_squotes(s[i], s_quote, d_quote);
		d_quote = check_dquotes(s[i], s_quote, d_quote);
		if (((s[i] == c || s[i] == '\"' || s[i] == '\'')
			&& s[i + 1] != 0 && s[i + 1] != c && !d_quote && !s_quote)
			|| (i && s[i - 1] != c && s[i - 1] != '\'' && s[i - 1] != '\"'
			&& (((s)[i] == '\'' && s_quote) || ((s)[i] == '\"' && d_quote))))
			num++;
		if ((s[i] == '\"' && s[i + 1] != 0 && s[i + 1] == '\"' && d_quote)
			|| (s[i] == '\'' && s[i + 1] != 0 && s[i + 1] == '\'' && s_quote))
			num--;
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

char	**split(char const *s, char c, char ***res)
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
	while (i < get_number_str(s, c))
	{
		num = 0;
		while (s[j] == c && s[j] != 0 && !d_quote && !s_quote)
			j++;
		while (s[j] != 0 && (d_quote || s_quote || s[j] != c))
		{
			s_quote = check_squotes(s[j], s_quote, d_quote);
			d_quote = check_dquotes(s[j], s_quote, d_quote);
			j++;
			num++;
		}
		(*res)[i] = ft_substr(s, j - num, num);
		if ((*res)[i] == NULL)
			return (malloc_fail(*res, i));
		i++;
	}
	(*res)[i] = 0;
	return (*res);
}

char	**ft_split2(char const *s, char c)
{
	char	**res;
	int		count;

	count = get_number_str(s, c);
	if (!count)
		return (NULL);
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (0);
	return (split(s, c, &res));
}

int main()
{
	// char **s = ft_split2("echo 'hi \" there'", ' ');
	// int i = 0;
	// while (s[i])
	// {
	// 	printf("%s\n", s[i]);
	// 	i++;
	// }
	char *s = "echo \'hi\'f\"\' \'\'\"\"\'\"\'there\'  ";
	printf("%s\n", s);
	printf("%d\n", get_number_str(s, ' '));
}
