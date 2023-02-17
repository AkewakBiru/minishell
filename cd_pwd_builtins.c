/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:41 by abiru             #+#    #+#             */
/*   Updated: 2023/02/17 22:17:42 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "./libft/libft.h"

// max file (dir) name length in bash is 255 bytes
char	*get_pwd(void)
{
	char	*cwd;
	char	*new;

	cwd = (char *)malloc(sizeof(char) * 65536);
	if (!cwd)
	{
		perror("");
		return (NULL);
	}
	if (!getcwd(cwd, 65536))
	{
		perror("");
		free(cwd);
		return (0);
	}
	new = ft_strdup(cwd);
	free(cwd);
	return (new);
}

void	print_pwd(void)
{
	char	*tmp;

	tmp = get_pwd();
	if (!tmp)
		return ;
	printf("%s\n", tmp);
	free(tmp);
}

/* 
	needs double check after finishing mkdir functionality
	expecially PWD and OLDPWD
*/

void	chg_dir(t_utils *cmd_utils, t_list **lst, t_list **export)
{
	char	*val;

	if (chdir(cmd_utils->cmd_arg[1]) == -1)
	{
		perror("cd");
		return ;
	}
	val = get_pwd();
	if (!val)
		return ;
	if (cmd_utils->key)
		free(cmd_utils->key);
	if (cmd_utils->value)
		free(cmd_utils->value);
	cmd_utils->key = ft_strdup("OLDPWD");
	cmd_utils->value = ft_strdup(get_val(lst, "PWD"));
	update_env(cmd_utils, lst);
	update_env(cmd_utils, export);
	if (cmd_utils->key)
		free(cmd_utils->key);
	if (cmd_utils->value)
		free(cmd_utils->value);
	cmd_utils->key = ft_strdup("PWD");
	cmd_utils->value = ft_strdup(val);
	update_env(cmd_utils, lst);
	update_env(cmd_utils, export);
	// update_dict(ft_strdup("OLDPWD"), ft_strdup(get_val(lst, "PWD")), lst);
	// update_dict(ft_strdup("OLDPWD"), ft_strdup(get_val(lst, "PWD")), export);
	// update_dict(ft_strdup("PWD"), ft_strdup(val), lst);
	// update_dict(ft_strdup("PWD"), ft_strdup(val), export);
	free(val);
}
