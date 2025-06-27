/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:22:34 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 18:32:31 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	export_variable(char *arg)
{
	int		has_equal;
	int		is_append;
	char	*name;
	char	*value;

	has_equal = (ft_strchr(arg, '=') != NULL);
	is_append = is_append_assignment(arg);
	if (has_equal)
	{
		name = get_substring_before_equal(arg);
		value = get_substring_after_equal(arg);
	}
	else
	{
		name = ft_strdup(arg);
		value = NULL;
	}
	if (variable_exists(name))
		handle_existing_variable(name, value, has_equal, is_append);
	else
		handle_new_variable(name, value, has_equal);
}

void	print_export_error(const char *arg)
{
	write(2, "bash: export: `", 15);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
}

static int	handle_export_args(char **args)
{
	int	i;
	int	has_error;

	i = 1;
	has_error = 0;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			export_variable(args[i]);
		else
		{
			has_error = 1;
			print_export_error(args[i]);
		}
		i++;
	}
	return (has_error);
}

static void	handle_export_no_args(int fd)
{
	char	**sorted_env;
	t_shell	*shell;
	int		n;

	(void)fd;
	shell = get_shell_instance();
	sorted_env = dup_env(shell->env);
	n = 0;
	while (sorted_env[n] != NULL)
		n++;
	quick_sort(sorted_env, 0, n - 1);
	print_env_in_bash_format(sorted_env, fd);
}

int	builtin_export(char **args, int fd)
{
	int	status;

	status = 0;
	if (args[1])
		status = handle_export_args(args);
	else
		handle_export_no_args(fd);
	return (status);
}
