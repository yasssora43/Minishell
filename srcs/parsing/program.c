/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   program.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:27 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/30 17:56:11 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	count_tokens(t_token *tokens)
{
	size_t	count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

t_token	*tokenize_and_clean(char *input)
{
	char	*fmt;
	t_token	*tokens;
	t_token	*tmp;

	if (!input || !*input || check_unclosed_quotes(input))
		return (NULL);
	fmt = input_formating(input);
	if (!fmt)
		return (NULL);
	tokens = input_analyse(fmt);
	if (!tokens)
		return (NULL);
	tmp = tokens;
	while (tmp)
	{
		if (tmp->value)
			tmp->value = remove_quotes(tmp->value);
		tmp = tmp->next;
	}
	return (tokens);
}

t_token	**tokenize_input(char *input, size_t *count)
{
	size_t	i;
	t_token	*tokens;
	t_token	**arr;

	tokens = tokenize_and_clean(input);
	if (!tokens)
		return (NULL);
	*count = count_tokens(tokens);
	arr = gc_malloc(sizeof(t_token *) * (*count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (tokens)
	{
		arr[i] = tokens;
		tokens = tokens->next;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

t_ast	*parser(char *input)
{
	size_t	count;
	t_token	**arr;
	t_ast	*ast;

	count = 0;
	arr = tokenize_input(input, &count);
	if (!arr || enhanced_parse_check(arr, count, input))
		return (NULL);
	ast = fill_tokens(arr);
	return (ast);
}
