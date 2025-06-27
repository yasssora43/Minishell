/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fii_ast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:37:38 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 16:36:56 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = gc_malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

t_ast	*fill_tokens(t_token **tokens)
{
	int			i;
	t_command	*cmd;
	t_ast		*node;
	t_ast		*right;

	i = 0;
	cmd = process_command_tokens(tokens, &i);
	if (!cmd)
		return (NULL);
	node = gc_malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->u_content.cmd = cmd;
	if (tokens[i] && tokens[i]->type == TOK_PIPE)
	{
		i++;
		right = fill_tokens(&tokens[i]);
		if (!right)
			return (NULL);
		return (create_pipe_node(node, right));
	}
	return (node);
}
