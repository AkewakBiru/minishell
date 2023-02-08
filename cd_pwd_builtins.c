/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 21:33:41 by abiru             #+#    #+#             */
/*   Updated: 2023/02/08 21:46:46 by abiru            ###   ########.fr       */
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
	printf("%s\n", tmp);
	free(tmp);
}

/* 
	needs double check after finishing mkdir functionality
*/
void	chg_dir(t_utils *cmd_utils, t_list **lst, t_list **export)
{
	if (chdir(cmd_utils->cmd_arg[1]) == -1)
	{
		perror("cd");
		return ;
	}
	cmd_utils->key = ft_strdup("PWD");
	cmd_utils->value = get_pwd();
	update_dict(cmd_utils, lst);
	update_dict(cmd_utils, export);
}
