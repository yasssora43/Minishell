/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:28:26 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 12:49:11 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*create_var(const char *name, const char *value, int has_equal)
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
	{
		write(2, "malloc error in add_new_variable\n", 33);
		return (NULL);
	}
	ft_memcpy(new_var, name, len[0]);
	if (has_equal)
		new_var[len[0]] = '=';
	if (value)
		ft_memcpy(new_var + len[0] + 1, value, len[1]);
	new_var[len[2]] = '\0';
	return (new_var);
}

void	update_variable_value(const char *name, const char \
								*value, int has_equal)
{
	char	**env;
	char	*new_var;
	int		i;
	int		name_len;

	env = get_shell_instance()->env;
	new_var = create_var(name, value, has_equal);
	i = 0;
	name_len = ft_strlen(name);
	if (!new_var)
		return ;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0 && (env[i][name_len] == \
			'=' || env[i][name_len] == '\0'))
		{
			env[i] = new_var;
			return ;
		}
		i++;
	}
	add_to_env(new_var);
}

void	append_variable_value(const char *name, const char *suffix, \
								int has_equal)
{
	char	**env;
	int		i;
	char	*old_value;
	char	*new_value;

	env = get_shell_instance()->env;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, ft_strlen(name)) == 0 && \
		env[i][ft_strlen(name)] == '=')
		{
			old_value = ft_strdup(env[i] + ft_strlen(name) + 1);
			new_value = ft_strjoin(old_value, suffix);
			update_variable_value(name, new_value, has_equal);
			return ;
		}
		i++;
	}
	add_new_variable(name, suffix, 1);
}

void	handle_existing_variable(char *name, char *value,
			int has_equal, int is_append)
{
	if (!has_equal)
		return ;
	if (is_append)
		append_variable_value(name, value, has_equal);
	else
		update_variable_value(name, value, has_equal);
}

void	handle_new_variable(char *name, char *value, int has_equal)
{
	if (value == NULL)
		add_new_variable(name, "", has_equal);
	else
		add_new_variable(name, value, has_equal);
}
