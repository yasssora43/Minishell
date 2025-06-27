/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_form.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:40:06 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/30 16:50:21 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	add_char(char **str, int *len, char c)
{
	char	*temp;
	int		i;

	temp = gc_malloc(*len + 2);
	if (!temp)
		return ;
	i = 0;
	while (i < *len)
	{
		temp[i] = (*str)[i];
		i++;
	}
	temp[(*len)++] = c;
	*str = temp;
}

static void	handle_redirection(char **result, char *input, int *i, int *j)
{
	char	redirection_char;

	redirection_char = input[*i];
	if (*j > 0 && (*result)[*j - 1] != ' ')
		add_char(result, j, ' ');
	add_char(result, j, redirection_char);
	(*i)++;
	if (input[*i] == redirection_char && redirection_char != '|')
	{
		add_char(result, j, input[*i]);
		(*i)++;
	}
	if (input[*i] && input[*i] != ' ')
		add_char(result, j, ' ');
}

static char	*handle_quotes(char *input, int *i, char *result, int *j)
{
	char	quote;

	quote = input[(*i)++];
	add_char(&result, j, quote);
	while (input[*i] && input[*i] != quote)
		add_char(&result, j, input[(*i)++]);
	if (input[*i] == quote)
		add_char(&result, j, input[(*i)++]);
	return (result);
}

static char	*process_input(char *input, int *final_len)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = gc_malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			result = handle_quotes(input, &i, result, &j);
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
			handle_redirection(&result, input, &i, &j);
		else
			add_char(&result, &j, input[i++]);
	}
	*final_len = j;
	return (result);
}

char	*input_formating(char *input)
{
	int		len;
	int		k;
	char	*processed;
	char	*final_str;

	if (!input)
		return (NULL);
	processed = process_input(input, &len);
	if (!processed)
		return (NULL);
	final_str = gc_malloc(len + 1);
	if (!final_str)
		return (NULL);
	k = 0;
	while (k < len)
	{
		final_str[k] = processed[k];
		k++;
	}
	final_str[len] = '\0';
	return (final_str);
}
