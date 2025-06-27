/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:34:16 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/29 17:31:09 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_ignore_sigint(struct sigaction *old_int)
{
	struct sigaction	ignore;

	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	sigaction(SIGINT, &ignore, old_int);
}

void	restore_sigint(struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
}

int	handle_heredoc_signal(int status, int *pipefd)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipefd[0]), -2);
	return (0);
}

int	write_pipe_to_file(const char *filename, int *pipefd)
{
	int		fd;
	char	buffer[1024];
	ssize_t	n;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (perror("heredoc file open"), close(pipefd[0]), -1);
	n = read(pipefd[0], buffer, sizeof(buffer));
	while (n > 0)
	{
		write(fd, buffer, n);
		n = read(pipefd[0], buffer, sizeof(buffer));
	}
	close(pipefd[0]);
	close(fd);
	return (0);
}
