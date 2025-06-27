/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:22:07 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 11:36:49 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **ptr, int i)
{
	(void)ptr;
	(void)i;
	return (NULL);
}

int	count_words(const char *s, char c)
{
	int	i;
	int	total_words;

	if (!s)
		return (-1);
	i = 0;
	total_words = 0;
	while (s[i])
	{
		while (s[i] && (c == s[i]))
			i++;
		if (s[i] && !(c == s[i]))
			total_words++;
		while (s[i] && !(c == s[i]))
			i++;
	}
	return (total_words);
}

char	*new_word(const char *s, int *index, char c)
{
	int		len;
	char	*word;
	int		strart_index;

	len = 0;
	strart_index = *index;
	while (s[*index] && !(c == s[*index]))
	{
		(*index)++;
		len++;
	}
	word = (char *)gc_malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	if (len > 0)
		ft_strlcpy(word, s + strart_index, len + 1);
	else
		word[0] = '\0';
	return (word);
}

char	**ft_split_words(char const *s, char c, char **s2, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < count)
	{
		while (s[i] && (c == s[i]))
			i++;
		if (s[i] && !(c == s[i]))
		{
			s2[j++] = new_word(s, &i, c);
			if (!s2[j - 1])
				return (free_array(s2, j - 1));
		}
		while (s[i] && !(c == s[i]))
			i++;
	}
	s2[j] = NULL;
	return (s2);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		count;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	strings = (char **)gc_malloc((count + 1) * sizeof(char *));
	if (!strings)
		return (NULL);
	strings = ft_split_words(s, c, strings, count);
	return (strings);
}
