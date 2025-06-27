/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 16:54:52 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 15:24:31 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	dup = gc_malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	is_var_char(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}

void	append_str(char *dst, const char *src, size_t *j)
{
	while (*src)
		dst[(*j)++] = *src++;
}

char	*get_key_value(const char *key, char **env)
{
	size_t	key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}
