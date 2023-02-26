/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 15:27:50 by abiru             #+#    #+#             */
/*   Updated: 2023/02/26 21:33:58 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	echo(char **arg)
{
	int	i;

	i = 1;
	if (!(arg + 1) || !arg[1])
	{
		printf("\n");
		return ;
	}
	if (arg + 1 && arg[i] && !ft_strcmp(arg[1], "-n"))
		i++;
	while (arg + i && arg[i])
	{
		printf("%s", arg[i]);
		if (arg + i + 1 && arg[i + 1])
			printf(" ");
		i++;
	}
	if (arg + 1 && arg[1] && ft_strcmp(arg[1], "-n") != 0)
		printf("\n");
	return ;
}
