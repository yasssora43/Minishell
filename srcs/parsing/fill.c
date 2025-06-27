/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:37:56 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 17:04:25 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_builtin_id	get_builtin_id(const char *arg)
{
	if (ft_strcmp(arg, "echo") == 0)
		return (BI_ECHO);
	else if (ft_strcmp(arg, "cd") == 0)
		return (BI_CD);
	else if (ft_strcmp(arg, "pwd") == 0)
		return (BI_PWD);
	else if (ft_strcmp(arg, "export") == 0)
		return (BI_EXPORT);
	else if (ft_strcmp(arg, "unset") == 0)
		return (BI_UNSET);
	else if (ft_strcmp(arg, "env") == 0)
		return (BI_ENV);
	else if (ft_strcmp(arg, "exit") == 0)
		return (BI_EXIT);
	return (BI_NONE);
}

void	handle_command_or_option(t_command *cmd, t_token *token)
{
	int		j;
	int		i;
	char	**new_args;

	if (token->type != TOK_WORD)
		return ;
	j = 0;
	i = 0;
	if (cmd->args)
		while (cmd->args[j])
			j++;
	new_args = gc_malloc(sizeof(char *) * (j + 2));
	if (!new_args)
		return ;
	while (i < j)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[j] = ft_strdup(token->value);
	new_args[j + 1] = NULL;
	cmd->args = new_args;
}

static t_redirect	*create_redirect_node(t_token *token, t_token *next)
{
	t_redirect	*new;

	new = gc_malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->heredoc_file = NULL;
	if (token->type == TOK_REDIR_IN)
		new->type = REDIR_IN;
	else if (token->type == TOK_REDIR_OUT)
		new->type = REDIR_OUT;
	else if (token->type == TOK_REDIR_APPEND)
		new->type = REDIR_APPEND;
	else if (token->type == TOK_REDIR_HEREDOC)
	{
		new->type = REDIR_HEREDOC;
		new->file = ft_strdup(next->value);
		new->delimiter_quoted = (next->quote_type != '\0');
		return (new);
	}
	new->file = ft_strdup(next->value);
	return (new);
}

void	handle_redirec(t_command *cmd, t_token *tok, t_token *next_tok, int *i)
{
	t_redirect	*new_redir;
	t_redirect	*last;

	new_redir = create_redirect_node(tok, next_tok);
	if (!new_redir)
		return ;
	if (!cmd->redirects)
		cmd->redirects = new_redir;
	else
	{
		last = cmd->redirects;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	*i += 2;
}

t_command	*process_command_tokens(t_token **tokens, int *i)
{
	t_command	*cmd;

	cmd = gc_malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->builtin_id = BI_NONE;
	while (tokens[*i] && tokens[*i]->type != TOK_PIPE)
	{
		if (tokens[*i]->type == TOK_WORD)
		{
			handle_command_or_option(cmd, tokens[*i]);
			if (cmd->args && cmd->args[0])
				cmd->builtin_id = get_builtin_id(cmd->args[0]);
			(*i)++;
		}
		else if (is_redirection(tokens[*i]))
			handle_redirec(cmd, tokens[*i], tokens[*i + 1], i);
		else
			return (NULL);
	}
	return (cmd);
}
