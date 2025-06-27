/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils6.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:03:07 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/18 10:07:03 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_env_var(const char *key, const char *value)
{
	char	*arg;
	char	*equal_sign;

	if (!key || !value)
		return (1);
	equal_sign = ft_strdup("=");
	if (!equal_sign)
		return (1);
	arg = gc_malloc(ft_strlen(key) + ft_strlen(value) + 2);
	if (!arg)
	{
		return (1);
	}
	ft_strcpy(arg, (char *)key);
	ft_strcat(arg, equal_sign);
	ft_strcat(arg, value);
	export_variable(arg);
	return (0);
}

static int	update_pwd_vars(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
		return (1);
	if (set_env_var("PWD", cwd) != 0)
		return (1);
	return (0);
}

static int	get_target_dir(t_shell *shell, char **args, char **dir, int fd)
{
	if (!args[1])
	{
		*dir = get_env_value2(shell->env, "HOME");
		if (!*dir)
		{
			write(fd, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
		*dir = args[1];
	return (0);
}

static int	change_directory(const char *dir, int fd)
{
	if (chdir(dir) != 0)
	{
		write(fd, strerror(errno), ft_strlen(strerror(errno)));
		write(fd, "\n", 1);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **args, int fd)
{
	t_shell	*shell;
	char	*dir;
	char	oldpwd[PATH_MAX];

	shell = get_shell_instance();
	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		write(fd, "cd: error getting current directory\n", 36);
		return (1);
	}
	if (get_target_dir(shell, args, &dir, fd) != 0)
		return (1);
	if (change_directory(dir, fd) != 0)
		return (1);
	if (set_env_var("OLDPWD", oldpwd) != 0)
	{
		write(fd, "cd: error setting OLDPWD\n", 26);
		return (1);
	}
	if (update_pwd_vars() != 0)
	{
		write(fd, "cd: error updating PWD\n", 23);
		return (1);
	}
	return (0);
}
