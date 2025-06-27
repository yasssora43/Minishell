/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:06:57 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/29 14:27:58 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	run_builtin(t_command *cmd, t_shell *shell)
{
	int	fd;
	int	status;

	(void)shell;
	if (!cmd || !cmd->args)
		return (1);
	fd = STDOUT_FILENO;
	if (cmd->builtin_id == BI_ECHO)
		status = builtin_echo(cmd->args, fd);
	else if (cmd->builtin_id == BI_CD)
		status = builtin_cd(cmd->args, fd);
	else if (cmd->builtin_id == BI_PWD)
		status = builtin_pwd(cmd->args, fd);
	else if (cmd->builtin_id == BI_EXPORT)
		status = builtin_export(cmd->args, fd);
	else if (cmd->builtin_id == BI_UNSET)
		status = builtin_unset(cmd->args, fd);
	else if (cmd->builtin_id == BI_ENV)
		status = builtin_env(cmd->args, fd);
	else if (cmd->builtin_id == BI_EXIT)
		status = builtin_exit(cmd->args, fd);
	else
		status = 1;
	return (status);
}

int	backup_fds(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
	if (*in < 0 || *out < 0)
	{
		perror("dup");
		return (-1);
	}
	return (0);
}

void	restore_fds(int in, int out)
{
	if (dup2(in, STDIN_FILENO) == -1 || dup2(out, STDOUT_FILENO) == -1)
		perror("restore_fds dup2");
	close(in);
	close(out);
}

void	reset_signals_to_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	interpret_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}
