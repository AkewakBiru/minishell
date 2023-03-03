/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_constructor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiru <abiru@student.42abudhabi.ae>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 20:46:56 by abiru             #+#    #+#             */
/*   Updated: 2023/03/03 10:13:00 by abiru            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_join_free(t_dict *dict)
{
	char	*new;
	char	*tmp;

	tmp = ft_strdup(dict->key);
	new = ft_strjoin(tmp, "=");
	free(tmp);
	tmp = ft_strjoin(new, dict->value);
	free(new);
	return (tmp);
}

char	**construct_envp(t_list **lst)
{
	t_list	*tmp;
	int		size;
	char	**envp;
	int		i;

	tmp = *lst;
	size = ft_lstsize(tmp);
	envp = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	if (!envp)
		return (0);
	while (tmp)
	{
		envp[i] = ft_join_free((t_dict *)tmp->content);
		tmp = tmp->next;
		i++;
	}
	envp[i] = 0;
	return (envp);
}

void	create_env(t_list **head, char **envp)
{
	int		i;
	t_list	*new;
	t_dict	*dict;

	i = -1;
	while (envp[++i])
	{
		dict = create_dict(ft_strndup(envp[i], '='),
				ft_strdup(ft_strchr(envp[i], '=') + 1), 1);
		new = ft_lstnew_dict(dict);
		ft_lstadd_back(head, new);
		free_dict(dict, 0);
	}
}
