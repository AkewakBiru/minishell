/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 15:27:50 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 00:53:30 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

int	is_n(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	if (str + i && str[i] && str[i] == '-')
		i++;
	else
		return (0);
	while (str + i && str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	echo(char **arg)
{
	int	i;

	i = 1;
	if (!(arg + 1) || !arg[1])
	{
		printf("\n");
		return (0);
	}
	while (arg + i && arg[i] && !ft_strncmp(arg[i], "-n", 2) && is_n(arg[i]))
		i++;
	while (arg + i && arg[i])
	{
		printf("%s", arg[i]);
		if (arg + i + 1 && arg[i + 1])
			printf(" ");
		i++;
	}
	if (arg + 1 && arg[1] && !is_n(arg[1]))
		printf("\n");
	return (0);
}
