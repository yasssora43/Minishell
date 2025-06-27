/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:08:00 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 12:58:06 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_path_validity(char *path, t_command *cmd)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		perror(cmd->args[0]);
		gc_free_all();
		get_shell_instance()->exit_status = 126;
		gc_free_all();
		exit(126);
	}
	if (S_ISDIR(st.st_mode))
	{
		print_cmd_error(cmd->args[0], ": Is a directory\n", 126);
		gc_free_all();
		exit(126);
	}
	if (access(path, X_OK) != 0)
		print_cmd_error(cmd->args[0], ": Permission denied\n", 126);
}

int	handle_builtin_command(t_command *cmd, t_shell *shell, \
				int has_redirects)
{
	int	original_in;
	int	original_out;

	if (has_redirects)
	{
		if (backup_fds(&original_in, &original_out))
			return (shell->exit_status = 1);
		if (apply_redirections(cmd->redirects) == -1)
		{
			restore_fds(original_in, original_out);
			return (shell->exit_status = 1);
		}
	}
	shell->exit_status = run_builtin(cmd, shell);
	if (has_redirects)
		restore_fds(original_in, original_out);
	return (shell->exit_status);
}

int	handle_redirections(t_command *cmd, int *in, int *out)
{
	if (backup_fds(in, out))
		return (1);
	if (apply_redirections(cmd->redirects) == -1)
	{
		restore_fds(*in, *out);
		return (1);
	}
	return (0);
}

int	execute_command(t_command *cmd, t_shell *shell, int in_pipeline)
{
	int	has_redirects;
	int	is_builtin;
	int	original_in;
	int	original_out;

	original_in = -1;
	original_out = -1;
	is_builtin = cmd->builtin_id != BI_NONE;
	has_redirects = has_redirections(cmd->redirects);
	if (is_builtin && !in_pipeline)
		return (handle_builtin_command(cmd, shell, has_redirects));
	if (has_redirects && handle_redirections(cmd, &original_in, &original_out))
		return (shell->exit_status = 1);
	if (!is_builtin || in_pipeline)
		shell->exit_status = run_external_or_forked_builtin(cmd, shell);
	if (has_redirects)
		restore_fds(original_in, original_out);
	return (shell->exit_status);
}
