/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:22:25 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/19 21:04:17 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **args, int fd)
{
	int	i;
	int	nl;
	int	first;

	i = 1;
	nl = 1;
	first = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		i++;
		nl = 0;
	}
	while (args[i])
	{
		if (write_with_space(fd, args[i], first))
			return (1);
		i++;
		first = 0;
	}
	if (nl)
		if (write(fd, "\n", 1) < 0)
			return (1);
	return (0);
}

int	builtin_pwd(char **args, int fd)
{
	t_shell	*shell;
	char	*pwd;

	(void)args;
	shell = get_shell_instance();
	pwd = get_env_value2(shell->env, "PWD");
	if (!pwd)
	{
		write(fd, "minishell: pwd: PWD not set\n", 29);
		return (1);
	}
	if (write(fd, pwd, ft_strlen(pwd)) < 0)
		return (1);
	if (write(fd, "\n", 1) < 0)
		return (1);
	return (0);
}
