/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:55 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 16:40:19 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_bool	is_delimiter(char c, const char *delimiters)
{
	while (*delimiters)
	{
		if (c == *delimiters)
			return (TRUE);
		delimiters++;
	}
	return (FALSE);
}

static t_bool	handle_quotes(char c, t_bool *in_quote, char *quote_char)
{
	if (!*in_quote && (c == '\'' || c == '"'))
	{
		*quote_char = c;
		*in_quote = TRUE;
		return (TRUE);
	}
	else if (*in_quote && c == *quote_char)
	{
		*in_quote = FALSE;
		return (TRUE);
	}
	return (FALSE);
}

static size_t	count_words(const char *str, const char *delimiters)
{
	size_t	count;
	t_bool	in_quote;
	char	quote_char;

	count = 0;
	in_quote = FALSE;
	quote_char = 0;
	while (*str)
	{
		while (*str && !in_quote && is_delimiter(*str, delimiters))
			str++;
		if (*str && (in_quote || !is_delimiter(*str, delimiters)))
		{
			count++;
			while (*str && (in_quote || !is_delimiter(*str, delimiters)))
			{
				handle_quotes(*str, &in_quote, &quote_char);
				str++;
			}
		}
	}
	return (count);
}

static void	process_word(char **ptr, char **result, size_t *i, char *delimiters)
{
	char	*start;
	t_bool	in_quote;
	char	quote_char;

	in_quote = FALSE;
	quote_char = 0;
	while (**ptr && !in_quote && is_delimiter(**ptr, delimiters))
		(*ptr)++;
	start = *ptr;
	while (**ptr && (in_quote || !is_delimiter(**ptr, delimiters)))
	{
		if (!in_quote && (**ptr == '\'' || **ptr == '"'))
		{
			in_quote = TRUE;
			quote_char = **ptr;
		}
		else if (in_quote && **ptr == quote_char)
			in_quote = FALSE;
		(*ptr)++;
	}
	result[*i] = (char *)gc_malloc(*ptr - start + 1);
	if (result[*i])
		ft_strlcpy(result[*i], start, *ptr - start + 1);
	(*i)++;
}

char	**ft_split2(char *str, char *delimiters)
{
	char	**result;
	size_t	i;
	size_t	words_count;
	char	*ptr;

	if (!str || !delimiters)
		return (NULL);
	words_count = count_words(str, delimiters);
	result = (char **)gc_malloc(sizeof(char *) * (words_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	ptr = str;
	while (*ptr && i < words_count)
		process_word(&ptr, result, &i, delimiters);
	result[i] = NULL;
	return (result);
}
