/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:08:27 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 13:01:59 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	child_process_left(t_ast *node, t_shell *shell, int *fds)
{
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		perror("restore_fds dup2");
		shell->exit_status = EXIT_FAILURE;
		gc_free_all();
		exit(EXIT_FAILURE);
	}
	close(fds[0]);
	close(fds[1]);
	shell->exit_status = execute_ast(node->left, shell, 1);
	gc_free_all();
	exit(shell->exit_status);
}

void	child_process_right(t_ast *node, t_shell *shell, int *fds)
{
	if (dup2(fds[0], STDIN_FILENO) == -1)
	{
		perror("restore_fds dup2");
		shell->exit_status = EXIT_FAILURE;
		gc_free_all();
		exit(EXIT_FAILURE);
	}
	close(fds[0]);
	close(fds[1]);
	shell->exit_status = execute_ast(node->right, shell, 1);
	gc_free_all();
	exit(shell->exit_status);
}

int	handle_first_child(t_ast *node, t_shell *shell, int *fds)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (close(fds[0]), close(fds[1]), perror("fork"), -1);
	if (pid == 0)
	{
		reset_signals_in_child();
		child_process_left(node, shell, fds);
	}
	return (pid);
}

int	handle_second_child(t_ast *node, t_shell *shell, int *fds, \
			pid_t first_pid)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(fds[0]);
		close(fds[1]);
		waitpid(first_pid, NULL, 0);
		return (perror("fork"), -1);
	}
	if (pid == 0)
	{
		reset_signals_in_child();
		child_process_right(node, shell, fds);
	}
	return (pid);
}
