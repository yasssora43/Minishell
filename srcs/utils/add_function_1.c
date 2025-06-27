/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_function_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:34:52 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 18:24:19 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

t_bool	is_redirection(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == TOK_REDIR_IN
		|| token->type == TOK_REDIR_OUT
		|| token->type == TOK_REDIR_APPEND
		|| token->type == TOK_REDIR_HEREDOC);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

char	*until_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	return (ft_substr(str, 0, i));
}

char	*extract_var_name(const char *str)
{
	int	i;

	if (!str || !*str)
		return (ft_strdup(""));
	if (*str == '?')
		return (ft_strdup("?"));
	if (!isalpha(*str) && *str != '_')
		return (ft_strdup(""));
	i = 1;
	while (str[i] && (isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, 0, i));
}
