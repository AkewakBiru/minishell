/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 14:30:07 by abiru             #+#    #+#             */
/*   Updated: 2023/02/26 21:31:27 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
	{
		if (arr[i])
			free(arr[i]);
	}
	if (arr)
		free(arr);
	arr = NULL;
}

char	*get_envp(char **envp)
{
	int		i;
	char	**path;
	char	*tmp;

	i = -1;
	tmp = NULL;
	while (envp[++i])
	{
		path = ft_split(envp[i], '=');
		if (ft_strncmp(path[0], "PATH", ft_strlen(path[0])) == 0)
		{
			if (path[1])
				tmp = ft_strdup(path[1]);
			free_arr(path);
			return (tmp);
		}
		free_arr(path);
	}
	return (NULL);
}

char	*get_cmd_path(char **env, char *cmd)
{
	char	*tmp;
	char	*cmd_path;
	int		i;

	i = -1;
	if (!cmd)
		return (NULL);
	while (env[++i])
	{
		tmp = ft_strjoin(env[i], "/");
		cmd_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
	}
	// printf("get here\n");
	return (ft_strdup(cmd));
}

char	**sep_cmdopt(char *str)
{
	char	**arr;

	if (!str)
		return (NULL);
	arr = ft_split(str, ' ');
	return (arr);
}

t_strs	*init_struct(void)
{
	t_strs	*t_str;

	t_str = (t_strs *)malloc(sizeof(t_strs));
	if (!t_str)
		return (0);
	t_str->ind_p = NULL;
	t_str->env_p = NULL;
	t_str->lim = NULL;
	t_str->cmd_args = NULL;
	t_str->tmp_cmd = NULL;
	t_str->tmp = NULL;
	return (t_str);
}
