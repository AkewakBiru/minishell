/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 15:27:50 by abiru             #+#    #+#             */
/*   Updated: 2023/02/16 17:33:40 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	echo(char **arg)
{
	if (!arg || !(*arg) || !(arg[1]))
	{
		printf("\n");
		return ;
	}
	if (!ft_strcmp(arg[1], "-n"))
	{
		if (arg[2])
			printf("%s", arg[2]);	
	}
	else if (arg[1])
		printf("%s\n", arg[1]);
}
