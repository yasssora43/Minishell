/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:19:04 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 12:48:50 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_var_with_equal(char *env_var, char *equals_pos, int fd)
{
	char	*name;
	char	*value;
	int		name_len;

	name_len = equals_pos - env_var;
	name = gc_malloc(name_len + 1);
	if (!name)
		return ;
	ft_memcpy(name, env_var, name_len);
	name[name_len] = '\0';
	value = equals_pos + 1;
	print_var_declaration(name, fd);
	if (value[0] == '\0')
		ft_putendl_fd("=", fd);
	else
		print_value(value, fd);
}

static void	handle_var_without_equal(char *env_var, int fd)
{
	print_var_declaration(env_var, fd);
	ft_putendl_fd("", fd);
}

static void	print_one_var(char *env_var, int fd)
{
	char	*equals_pos;

	if (!env_var || fd < 0)
		return ;
	equals_pos = ft_strchr(env_var, '=');
	if (equals_pos)
		handle_var_with_equal(env_var, equals_pos, fd);
	else
		handle_var_without_equal(env_var, fd);
}

void	print_env_in_bash_format(char **env, int fd)
{
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		print_one_var(env[i], fd);
		i++;
	}
}

int	is_valid_identifier(const char *str)
{
	int	i;
	int	plus_found;

	plus_found = 0;
	if (!str || (!isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+')
		{
			if (plus_found || str[i + 1] != '=')
				return (0);
			plus_found = 1;
		}
		else if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
