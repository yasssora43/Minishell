/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:07:31 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 13:03:37 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_parent_process(pid_t pid, t_shell *shell, t_command *cmd)
{
	struct sigaction	old_int;
	struct sigaction	sa_ignore;
	int					status;
	int					sig;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &old_int);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	shell->exit_status = interpret_exit_status(status);
	cleanup_heredoc_files(cmd->redirects);
	return (shell->exit_status);
}

void	execute_external(t_command *cmd, t_shell *shell)
{
	char	*path;

	path = get_command_path(cmd->args[0], shell->env);
	if (!path)
		print_cmd_error(cmd->args[0], ": command not found\n", 127);
	check_path_validity(path, cmd);
	reset_signals_in_child();
	execve(path, cmd->args, shell->env);
	perror(cmd->args[0]);
	gc_free_all();
	get_shell_instance()->exit_status = 126;
	gc_free_all();
	exit(126);
}

int	run_child_process(t_command *cmd, t_shell *shell)
{
	if (apply_redirections(cmd->redirects) == -1)
	{
		get_shell_instance()->exit_status = 1;
		gc_free_all();
		exit(1);
	}
	if (cmd->builtin_id != BI_NONE)
	{
		gc_free_all();
		exit (run_builtin(cmd, shell));
	}
	execute_external(cmd, shell);
	perror(cmd->args[0]);
	return (127);
}

int	run_external_or_forked_builtin(t_command *cmd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		reset_signals_in_child();
		run_child_process(cmd, shell);
	}
	return (handle_parent_process(pid, shell, cmd));
}

void	print_cmd_error(const char *cmd, const char *msg, int code)
{
	write(2, "leetshell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, msg, ft_strlen(msg));
	gc_free_all();
	get_shell_instance()->exit_status = code;
	gc_free_all();
	exit(code);
}
