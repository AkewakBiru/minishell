/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 21:06:46 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/03 21:10:08 by yel-touk         ###   ########.fr       */
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

int	is_syntax_error(int index, t_token ***tokens)
{
	if (((*tokens)[index]->type == pip && ((*tokens)[index - 1]->type == pip
		|| !(*tokens)[index + 1])) || ((*tokens)[index]->type == redirection
			&& label_redirection((*tokens)[index]))
			|| (is_redirection((*tokens)[index]->type) && (!(*tokens)[index + 1]
			|| !((*tokens)[index + 1]->type == unset
			|| (*tokens)[index + 1]->type == delimiter_q)
			|| ((*tokens)[index]->type != here_doc
			&& !valid_file((*tokens)[index + 1]->token)))))
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
	if (name[0] == '/' || name[0] == '.' || name[0] == ':')
		return (0);
	return (1);
}
