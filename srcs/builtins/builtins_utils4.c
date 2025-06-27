/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:23:23 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 18:31:26 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_new_variable(const char *name, const char *value, int has_equal)
{
	char	*new_var;
	int		len[3];

	len[0] = ft_strlen(name);
	if (value != NULL)
		len[1] = ft_strlen(value);
	else
		len[1] = 0;
	len[2] = len[0] + len[1];
	if (has_equal)
		len[2]++;
	new_var = gc_malloc(len[2] + 1);
	if (!new_var)
		return ;
	ft_memcpy(new_var, name, len[0]);
	if (has_equal)
		new_var[len[0]] = '=';
	if (value)
		ft_memcpy(new_var + len[0] + 1, value, len[1]);
	new_var[len[2]] = '\0';
	add_to_env(new_var);
}

char	*get_substring_before_equal(const char *arg)
{
	int		i;
	char	*before_equal;

	i = 0;
	while (arg[i] && !(arg[i] == '=' || (arg[i] == '+' && arg[i + 1] == '=')))
		i++;
	before_equal = gc_malloc(i + 1);
	if (!before_equal)
		return (NULL);
	ft_memcpy(before_equal, arg, i);
	before_equal[i] = '\0';
	return (before_equal);
}

char	*get_substring_after_equal(const char *arg)
{
	int		i;

	i = 0;
	while (arg[i] && !(arg[i] == '=' || (arg[i] == '+' && arg[i + 1] == '=')))
		i++;
	if (arg[i] == '\0')
		return (NULL);
	if (arg[i] == '+')
		i++;
	i++;
	return (ft_strdup(&arg[i]));
}

int	variable_exists(const char *name)
{
	char	**env;
	int		i;
	size_t	len;

	env = get_shell_instance()->env;
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && (env[i][len] \
		== '=' || env[i][len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

int	is_append_assignment(const char *arg)
{
	char	*plus_equal;

	plus_equal = ft_strstr((char *)arg, "+=");
	return (plus_equal != NULL && plus_equal != arg);
}
