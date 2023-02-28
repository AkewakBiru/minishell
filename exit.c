/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:50:47 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 18:00:00 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	str = 0;
	return ;
}

/*
* Added "*" before tokens because of compilation error
*/

int	count_arg(char **args)
{
	int	i;

	i = 0;
	while (args + i && args[i])
		i++;
	return (i);
}

int	check_digit(char *arg)
{
	int	i;

	i = 0;
	if (arg + i && (arg[i] == '-' || arg[i] == '+'))
		i++;
	while (arg + i && arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int exit_shell(t_list *env_pack[2], char **cmd_args, t_ints *t_int, int is_child)
{
	unsigned long long	tmp;

	tmp = 0;
	if (count_arg(cmd_args) > 2)
	{
		ft_putendl_fd("bash: exit: too many arguments", 2);
		exit_status = 1;
		return (1);
	}
	if (!is_child)
		ft_putendl_fd("exit", 2);
	if (cmd_args + 1 && cmd_args[1])
		tmp = ft_atoi(cmd_args[1]);
	if (cmd_args + 1 && (!check_digit(cmd_args[1]) || tmp > 9223372036854775808ULL))
		exit_status = error_msg("numeric argument required", cmd_args, 1, 255);
	if (tmp <= 9223372036854775808ULL)
		exit_status = tmp % 256;
	ft_lstclear_dict(env_pack + 0, free);
	ft_lstclear_dict(env_pack + 1, free);
	free_arr(cmd_args);
	if (t_int->pipes)
		free(t_int->pipes);
	close(t_int->RLSTDIN);
	close(t_int->RLSTDOUT);
	exit(exit_status);
}
