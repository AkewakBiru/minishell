/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 09:08:17 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 20:23:34 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_shl_val(t_list *envp[2], t_dict *dict, unsigned long long n)
{
	if (get_val(&(envp)[0], "SHLVL"))
	{
		n = custom_atoi(get_val(&(envp)[0], "SHLVL"));
		if (n >= 9999 && n <= 2147483646)
		{
			n++;
			ft_putstr_fd("bash: warning: shell level (", 2);
			ft_putnbr_fd(n, 2);
			ft_putendl_fd(") too high, resetting to 1", 2);
			n = 0;
		}
		else if (n > 2147483646)
			n = -1;
		n++;
	}
	else
		n = 1;
	dict->value = ft_itoa(n);
	update_env(dict, &(envp)[0]);
	update_env(dict, &(envp)[1]);
	free_dict(dict, 0);
	return (0);
}

int	update_shell(t_list *envp[2])
{
	unsigned long long	n;
	t_dict				*dict;

	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
	{
		perror("malloc");
		return (1);
	}
	dict->key = ft_strdup("SHLVL");
	dict->flag = 1;
	n = 0;
	if (key_exists("SHLVL", &(envp)[0]))
		return (check_shl_val(envp, dict, n));
	n = 1;
	dict->value = ft_itoa(n);
	update_env(dict, &(envp)[0]);
	update_env(dict, &(envp)[1]);
	free_dict(dict, 0);
	return (0);
}
