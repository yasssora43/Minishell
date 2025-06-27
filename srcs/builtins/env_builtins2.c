/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:13:40 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 18:23:09 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_var_declaration(char *var, int fd)
{
	ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(var, fd);
}

void	print_value(char *value, int fd)
{
	ft_putstr_fd("=\"", fd);
	ft_putstr_fd(value, fd);
	ft_putendl_fd("\"", fd);
}

static int	is_valid_variable(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	**remove_var_from_env(char **env, const char *name)
{
	char	**new_env;
	int		i;
	int		j;
	size_t	len;

	len = ft_strlen(name);
	i = 0;
	j = 0;
	while (env[i])
		i++;
	new_env = gc_malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = -1;
	while (env[++i])
	{
		if (!(ft_strncmp(env[i], name, len) == 0 && (env[i][len] == '=' \
					|| env[i][len] == '\0')))
			new_env[j++] = ft_strdup(env[i]);
	}
	new_env[j] = NULL;
	return (new_env);
}

int	builtin_unset(char **args, int fd)
{
	int		i;
	char	**new_env;
	t_shell	*shell;

	(void)fd;
	shell = get_shell_instance();
	i = 1;
	while (args[i])
	{
		if (is_valid_variable(args[i]) && variable_exists(args[i]))
		{
			new_env = remove_var_from_env(shell->env, args[i]);
			if (!new_env)
				return (0);
			shell->env = new_env;
		}
		i++;
	}
	return (0);
}
