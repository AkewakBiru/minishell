/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:41 by abiru             #+#    #+#             */
/*   Updated: 2023/02/28 17:59:11 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// max file (dir) name length in bash is 255 bytes
char	*get_pwd(void)
{
	char	*cwd;

	cwd = 0;
	cwd = getcwd(cwd, 0);
	if (!cwd)
	{
		perror("");
		return (0);
	}
	return (cwd);
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

int	chg_dir(char **cmd_utils, t_list **lst, t_list **export, t_ints *t_int)
{
	char	*val;
	t_dict	*dict;

	if (cmd_utils + 1 && cmd_utils[1] && chdir(cmd_utils[1]) == -1)
	{
		perror("cd");
		t_int->e_status = 1;
		return (1);
	}
	val = get_pwd();
	if (!val)
		return (1);
	dict = (t_dict *)malloc(sizeof(t_dict));
	if (!dict)
		return (1);
	dict->key = ft_strdup("OLDPWD");
	dict->value = ft_strdup(get_val(lst, "PWD"));
	dict->flag = 1;
	update_env(dict, lst);
	update_env(dict, export);
	if (dict->key)
		free(dict->key);
	if (dict->value)
		free(dict->value);
	dict->key = ft_strdup("PWD");
	dict->value = ft_strdup(val);
	update_env(dict, lst);
	update_env(dict, export);
	free(val);
	return (0);
}
