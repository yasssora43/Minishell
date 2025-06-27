/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:24:16 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 17:30:32 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_and_handle_signals(pid_t *pid, t_shell *shell)
{
	int					status1;
	int					status2;
	int					sig;
	struct sigaction	sa_ignore;
	struct sigaction	sa_old;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_old);
	waitpid(pid[0], &status1, 0);
	waitpid(pid[1], &status2, 0);
	if (WIFSIGNALED(status2))
	{
		sig = WTERMSIG(status2);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	sigaction(SIGINT, &sa_old, NULL);
	setup_signals();
	shell->exit_status = interpret_exit_status(status2);
}

int	execute_pipeline(t_ast *node, t_shell *shell)
{
	int		fds[2];
	pid_t	pid[2];

	if (pipe(fds) < 0)
		return (perror("pipe"), 1);
	pid[0] = handle_first_child(node, shell, fds);
	if (pid[0] < 0)
		return (1);
	pid[1] = handle_second_child(node, shell, fds, pid[0]);
	if (pid[1] < 0)
		return (1);
	close(fds[0]);
	close(fds[1]);
	wait_and_handle_signals(pid, shell);
	return (shell->exit_status);
}

int	execute_ast(t_ast *node, t_shell *shell, int in_pipeline)
{
	if (!node)
		return (1);
	if (node->type == NODE_COMMAND)
	{
		return (execute_command(node->u_content.cmd, shell, in_pipeline));
	}
	else if (node->type == NODE_PIPE)
	{
		shell->in_pipeline = 1;
		return (execute_pipeline(node, shell));
	}
	else
	{
		printf("Unknown node type: %d\n", node->type);
		return (1);
	}
}

int	execute_shell(t_ast *ast, t_shell *shell)
{
	if (preprocess_heredocs_in_ast(ast) == -1)
		return (shell->exit_status = 1);
	return (execute_ast(ast, shell, 0));
}
