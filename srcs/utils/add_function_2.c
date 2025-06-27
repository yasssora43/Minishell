/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_function_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:35:19 by moelgham          #+#    #+#             */
/*   Updated: 2025/05/28 18:28:06 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_var_name(const char *str)
{
	return (ft_strlen(extract_var_name(str)));
}

int	find_var_in_env(const char *var)
{
	size_t	len;
	int		i;

	if (get_shell_instance()->env)
		return (-1);
	len = ft_strlen(var);
	i = 0;
	while (get_shell_instance()->env[i])
	{
		if (ft_strncmp(get_shell_instance()->env[i], var, len) == 0
			&& get_shell_instance()->env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*add_something(const char *s1, const char *s2)
{
	return (ft_strjoin(s1, s2));
}

char	*get_until_dollar(const char *str)
{
	return (until_dollar(str));
}

char	*ft_copy(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	return (ft_substr(str, start, *i - start));
}
