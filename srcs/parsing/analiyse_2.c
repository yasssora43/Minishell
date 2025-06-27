/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analiyse_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:36:57 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/31 18:24:18 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_command_exists(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOK_WORD)
			return (1);
		current = current->next;
	}
	return (0);
}

void	handle_redirection_tokens(t_token *token)
{
	if (!token || !token->value)
		return ;
	if (ft_strcmp(token->value, "<<") == 0)
		token->type = TOK_REDIR_HEREDOC;
	else if (ft_strcmp(token->value, "<") == 0)
		token->type = TOK_REDIR_IN;
	else if (ft_strcmp(token->value, ">") == 0)
		token->type = TOK_REDIR_OUT;
	else if (ft_strcmp(token->value, ">>") == 0)
		token->type = TOK_REDIR_APPEND;
}

void	handle_contextual_tokens(t_token *token, t_token *prv, int cmd_found)
{
	(void)cmd_found;
	if (!token)
		return ;
	if (!prv)
	{
		token->type = TOK_WORD;
		return ;
	}
	if (is_redirection(prv))
		token->type = TOK_WORD;
	else if (token->value[0] == '-' && token->value[1] != '\0'
		&& prv->type == TOK_WORD)
		token->type = TOK_WORD;
	else if (prv->type == TOK_WORD)
		token->type = TOK_WORD;
}

t_bool	should_expand(const char *str)
{
	t_bool	in_single;
	t_bool	in_double;
	int		i;

	in_single = FALSE;
	in_double = FALSE;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		if (str[i] == '$' && !in_single)
			return (TRUE);
		i++;
	}
	return (FALSE);
}

t_bool	set_basic_token_types(t_token *token, t_token *prv)
{
	if (ft_strcmp(token->value, "|") == 0)
	{
		token->type = TOK_PIPE;
		return (TRUE);
	}
	if (ft_strcmp(token->value, "<<") == 0 || ft_strcmp(token->value, ">>") == 0
		|| ft_strcmp(token->value, "<") == 0
		|| ft_strcmp(token->value, ">") == 0)
	{
		handle_redirection_tokens(token);
		return (TRUE);
	}
	if (prv && prv->type == TOK_REDIR_HEREDOC)
	{
		token->type = TOK_WORD;
		if (token->value[0] == '\'' || token->value[0] == '"')
			token->quote_type = token->value[0];
		else
			token->quote_type = 0;
		return (TRUE);
	}
	return (FALSE);
}
