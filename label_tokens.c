/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   label_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-touk <yel-touk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 13:42:13 by yel-touk          #+#    #+#             */
/*   Updated: 2023/03/04 20:21:27 by yel-touk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_first_token(int *index, int *begins_w_redir,
			t_token ***tokens, int flag)
{
	if ((*tokens)[*index]->type == unset)
	{
		(*tokens)[*index]->type = cmd;
		*begins_w_redir = 0;
		if (flag)
			*index += 1;
	}
	else
		*begins_w_redir = 1;
}

void	set_delimiter(int index, int begins_w_redir, t_token ***tokens)
{
	if ((*tokens)[index]->type == unset)
		(*tokens)[index]->type = delimiter;
	if (begins_w_redir && (*tokens)[index + 1]
		&& (*tokens)[index + 1]->type == unset)
		(*tokens)[index + 1]->type = cmd;
}

void	set_redirection_arg(int index, int begins_w_redir, t_token ***tokens)
{
	(*tokens)[index]->type = file;
	if (begins_w_redir && (*tokens)[index + 1]
		&& (*tokens)[index + 1]->type == unset)
		(*tokens)[index + 1]->type = cmd;
}

int	is_redirection_arg(int index, t_token ***tokens)
{
	if (((*tokens)[index]->type == unset
		|| (*tokens)[index]->type == delimiter_q)
		&& is_redirection((*tokens)[index - 1]->type)
		&& (*tokens)[index - 1]->type != here_doc)
		return (1);
	return (0);
}

int	label_tokens(t_token ***tokens)
{
	int	i;
	int	begins_w_redir;

	i = 0;
	if ((*tokens)[i] && (*tokens)[i]->type == pip)
		return (1);
	set_first_token(&i, &begins_w_redir, tokens, 1);
	while ((*tokens)[i])
	{
		if (is_syntax_error(i, tokens))
			return (1);
		if (i > 0 && (*tokens)[i - 1]->type == pip)
			set_first_token(&i, &begins_w_redir, tokens, 0);
		else if ((*tokens)[i]->token[0] == '-')
			(*tokens)[i]->type = option;
		else if (i > 0 && (*tokens)[i - 1]->type == here_doc)
			set_delimiter(i, begins_w_redir, tokens);
		else if (is_redirection_arg(i, tokens))
			set_redirection_arg(i, begins_w_redir, tokens);
		else if ((*tokens)[i]->type == unset)
			(*tokens)[i]->type = arg;
		i++;
	}
	return (0);
}
