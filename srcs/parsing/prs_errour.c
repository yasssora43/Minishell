/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_errour.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:42 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 16:40:15 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool	has_redirection_error(t_token **tokens, size_t count)
{
	size_t	i;
	t_token	*current;
	t_token	*next;

	i = 0;
	while (i + 1 < count)
	{
		current = tokens[i];
		next = tokens[i + 1];
		if ((current->type >= TOK_REDIR_OUT
				&& current->type <= TOK_REDIR_HEREDOC)
			&& (next->type != TOK_WORD))
		{
			ft_putstr_fd("syntax error near unexpected token\n", 2);
			return (TRUE);
		}
		i++;
	}
	if (count > 0
		&& tokens[count - 1]->type >= TOK_REDIR_OUT
		&& tokens[count - 1]->type <= TOK_REDIR_HEREDOC)
		return (TRUE);
	return (FALSE);
}

int	check_token_sequence(t_token *current, t_token *next)
{
	if (!current)
		return (-1);
	if ((current->type >= TOK_REDIR_OUT && current->type <= TOK_REDIR_HEREDOC)
		&& (!next || next->type != TOK_WORD))
		return (1);
	if (current->type == TOK_PIPE && !next)
		return (2);
	if ((current->type == TOK_AND || current->type == TOK_OR) && !next)
		return (3);
	return (-1);
}

t_bool	validate_initial_tokens(t_token **tokens, size_t count)
{
	if (count == 0)
		return (FALSE);
	if (tokens[0]->type == TOK_PIPE || tokens[0]->type == TOK_AND
		|| tokens[0]->type == TOK_OR)
	{
		return (err("minishell: syntax error near unexpected token ",
				tokens[0]->value, "'\n"));
	}
	if (tokens[count - 1]->type == TOK_PIPE
		|| tokens[count - 1]->type == TOK_REDIR_OUT
		|| tokens[count - 1]->type == TOK_REDIR_IN
		|| tokens[count - 1]->type == TOK_REDIR_APPEND
		|| tokens[count - 1]->type == TOK_REDIR_HEREDOC)
	{
		return (err("minishell: syntax error near unexpected token ",
				tokens[count - 1]->value, "'\n"));
	}
	return (FALSE);
}

static t_bool	handle_token_error(int err_type, t_token *cur)
{
	if (err_type == 1)
		return (err("minishell: error: missing after redirection ",
				cur->value, "'\n"));
	if (err_type == 2)
		return (err("minishell: error: missing after pipe ",
				cur->value, "'\n"));
	if (err_type == 3)
		return (err("minishell: syntax error: after operator `",
				cur->value, "'\n"));
	return (FALSE);
}

t_bool	validate_token(t_token **tokens, size_t count)
{
	size_t	i;
	t_token	*cur;
	t_token	*next;
	int		err_type;

	i = 0;
	while (i < count)
	{
		cur = tokens[i];
		if (i + 1 < count)
			next = tokens[i + 1];
		else
			next = NULL;
		err_type = check_token_sequence(cur, next);
		if (err_type != 0)
			return (handle_token_error(err_type, cur));
		if (next && is_consecutive_operators(cur, next))
			return (err("minishell: syntax error near token ",
					next->value, "'\n"));
		i++;
	}
	return (FALSE);
}
