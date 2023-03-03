/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 21:35:43 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 20:13:27 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	custom_err_msg(char *cmd, char *arg)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	error_msg(char *msg, char **args, int num, int err)
{
	ft_putstr_fd("bash: ", 2);
	if ((num == 1 || num == 2) && args + 0 && args[0])
		ft_putstr_fd(args[0], 2);
	if (args + 1 && args[1] && num != 2)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(args[1], 2);
	}
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (err);
}

void	ex_fail_msg(t_cmd_op *cmd, char **args, t_ints *t_int)
{
	DIR	*dir;

	dir = opendir(cmd->cmd);
	if (dir)
	{
		t_int->e_status = error_msg("is a directory", args, 2, 126);
		closedir(dir);
	}
	else if (ft_strchr(cmd->cmd, '/')
		&& access(cmd->cmd, F_OK) != 0)
		t_int->e_status = error_msg("No such file or directory", args, 2, 127);
	else if (ft_strchr(cmd->cmd, '/') && access(cmd->cmd, X_OK) != 0
		&& access(cmd->cmd, F_OK) == 0)
		t_int->e_status = error_msg("permission denied", args, 2, 126);
	else if (access(cmd->cmd, X_OK) == 0 && ft_strchr(cmd->cmd, '/'))
		return ;
	else
		t_int->e_status = error_msg("command not found", args, 2, 127);
}
