/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyise_cammnd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:37:20 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/21 11:34:08 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	set_tokens_type(t_token *token, t_token *prv, t_token *tokens)
{
	char	*expanded;
	int		cmd_found;

	if (!token)
		return ;
	if (set_basic_token_types(token, prv))
		return ;
	if (should_expand(token->value) && token->quote_type != '\'')
	{
		expanded = NULL;
		if (token->type == TOK_REDIR_HEREDOC)
			expanded = get_env_in_herdoc(token->value,
					get_shell_instance()->env, TRUE, token->value);
		else
			expanded = expand_token(token->value,
					get_shell_instance()->env, TRUE);
		if (expanded)
		{
			token->value = expanded;
		}
	}
	cmd_found = is_command_exists(tokens);
	handle_contextual_tokens(token, prv, cmd_found);
}

t_token	*create_token_and_link(char *val, t_token **prv, t_token **hed)
{
	t_token	*new;

	new = gc_malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(t_token));
	new->value = ft_strdup(val);
	if (!new->value)
		return (NULL);
	new->next = NULL;
	set_tokens_type(new, *prv, *hed);
	if (!*hed)
		*hed = new;
	else if (*prv)
		(*prv)->next = new;
	*prv = new;
	return (new);
}

t_token	*input_analyse(char *str)
{
	char	**words;
	t_token	*head;
	t_token	*prev;
	int		i;

	words = ft_split2(str, " ");
	if (!words)
		return (NULL);
	head = NULL;
	prev = NULL;
	i = -1;
	while (words[++i])
		if (!create_token_and_link(words[i], &prev, &head))
			return (NULL);
	return (head);
}
