/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:50:47 by abiru             #+#    #+#             */
/*   Updated: 2023/03/02 09:46:30 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

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
	if (arg + i && arg[i] == '\0')
		return (0);
	while (arg + i && arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ultimate_free(t_list *env_pack[2], t_cmd_op **cmd, t_ints *t_int)
{
	ft_lstclear_dict(env_pack + 0, free);
	ft_lstclear_dict(env_pack + 1, free);
	free_cmd_params(cmd);
	if (t_int->pipes)
		free(t_int->pipes);
	close(t_int->RLSTDIN);
	close(t_int->RLSTDOUT);
}

int	exit_shell(t_list *env_pack[2], t_cmd_op **cmd, t_ints *t_int, int is_child)
{
	unsigned long long	tmp;

	tmp = 0;
	if (count_arg(cmd[t_int->counter]->cmd_args) > 2)
	{
		ft_putendl_fd("bash: exit: too many arguments", 2);
		t_int->e_status = 1;
		return (1);
	}
	if (!is_child)
		ft_putendl_fd("exit", 2);
	if (cmd[t_int->counter]->cmd_args + 1 && cmd[t_int->counter]->cmd_args[1])
		tmp = ft_atoi(cmd[t_int->counter]->cmd_args[1]);
	if (cmd[t_int->counter]->cmd_args + 1
		&& (!check_digit(cmd[t_int->counter]->cmd_args[1])
			|| tmp > 9223372036854775808ULL))
		t_int->e_status = error_msg("numeric argument required",
				cmd[t_int->counter]->cmd_args, 1, 255);
	if (tmp <= 9223372036854775808ULL)
		t_int->e_status = tmp % 256;
	ultimate_free(env_pack, cmd, t_int);
	exit(t_int->e_status);
}
