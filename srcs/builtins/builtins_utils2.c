/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yassin <yassin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 19:40:46 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/18 18:39:39 by yassin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_n_flag(char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

int	ft_isnumeric(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	write_with_space(int fd, char *str, int first)
{
	if (!first)
		if (write(fd, " ", 1) < 0)
			return (1);
	if (write(fd, str, ft_strlen(str)) < 0)
		return (1);
	return (0);
}

void	add_to_env(char *new_var)
{
	t_shell	*shell;
	char	**new_env;
	int		i;

	shell = get_shell_instance();
	i = 0;
	while (shell->env[i])
		i++;
	new_env = gc_malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	i = 0;
	while (shell->env[i])
	{
		new_env[i] = ft_strdup(shell->env[i]);
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	shell->env = new_env;
}

int	builtin_env(char **args, int fd)
{
	int		i;
	t_shell	*shell;

	shell = get_shell_instance();
	if (args[1])
	{
		ft_putendl_fd("env: too many arguments", 2);
		return (1);
	}
	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
			ft_putendl_fd(shell->env[i], fd);
		i++;
	}
	return (0);
}
