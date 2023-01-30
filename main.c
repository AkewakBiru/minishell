/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 09:55:55 by abiru             #+#    #+#             */
/*   Updated: 2023/01/30 18:02:48 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

int main(int ac, char **av)
{
	(void)av;
	if (ac != 1)
		return (1);
	while (1)
	{
		// printf("Minishell> ");
		char *line = readline("Minishell> ");
		// char *line = "helo";
		printf("%s\n",line);
	}
	printf("here\n");
	
}

/*
pivoting is when a company changes some aspects of its core products or services.
it could be done to better meet customer demand, shift target audience.
sometimes pivoting goes well and other times it goes bad. So, before pivoting, 
we should measure its benefits and drawbacks.
*/
