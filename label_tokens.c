/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 13:42:13 by yel-touk          #+#    #+#             */
/*   Updated: 2023/02/20 13:04:52 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	label_redirection(t_token *token)
{
	if (!ft_strcmp(token->token, "<"))
		token->type = redir_in;
	if (!ft_strcmp(token->token, ">"))
		token->type = redir_out;
	if (!ft_strcmp(token->token, "<<"))
		token->type = here_doc;
	if (!ft_strcmp(token->token, ">>"))
		token->type = redir_out_append;
	if (token->type == redirection)
		return (1);
	return (0);
}

int	is_redirection(enum e_input_type type)
{
	if (type == redir_in || type == redir_out || type == here_doc
		|| type == redir_out_append)
		return (1);
	return (0);
}

int	valid_file(char *name)
{
	if (name[0] == '\0' || name[0] == '/' || name[0] == '.' || name[0] == ':')
		return (0);
	return (1);
}

int	label_tokens(t_token ***tokens)
{
	int	i;

	i = 0;
	if ((*tokens)[i] && (*tokens)[i]->type == pip)
		return (1);
	if ((*tokens)[i]->type == unset)
		(*tokens)[i++]->type = cmd;
	while ((*tokens)[i])
	{
		if (((*tokens)[i]->type == pip && (*tokens)[i - 1]->type == pip) //(is_redirection((*tokens)[i - 1]->type) || (*tokens)[i - 1]->type == pip))
			|| ((*tokens)[i]->type == redirection && label_redirection((*tokens)[i]))
			|| (is_redirection((*tokens)[i]->type) && (!(*tokens)[i + 1] || (*tokens)[i + 1]->type != unset || ((*tokens)[i]->type != here_doc && !valid_file((*tokens)[i + 1]->token))))) // is_redirection((*tokens)[i - 1]->type))
			return (1);
		if ((*tokens)[i]->type == unset && (*tokens)[i - 1]->type == pip)
			(*tokens)[i]->type = cmd;
		else if ((*tokens)[i]->token[0] == '-')
			(*tokens)[i]->type = option;
		else if ((*tokens)[i]->type == unset && (*tokens)[i - 1]->type == here_doc)
			(*tokens)[i]->type = delimiter;
		else if ((*tokens)[i]->type == unset && is_redirection((*tokens)[i - 1]->type))
			(*tokens)[i]->type = file;
		else if ((*tokens)[i]->type == unset)
			(*tokens)[i]->type = arg;
		i++;
	}
	return (0);
}