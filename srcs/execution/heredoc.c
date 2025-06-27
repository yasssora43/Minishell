/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 23:09:20 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 17:33:49 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_parent(pid_t pid, int *pipefd, const char *filename)
{
	int					status;
	struct sigaction	old_int;

	setup_ignore_sigint(&old_int);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	restore_sigint(&old_int);
	if (handle_heredoc_signal(status, pipefd) != 0)
		return (-2);
	return (write_pipe_to_file(filename, pipefd));
}

int	read_heredoc_to_file(const char *delimiter, const char *filename, \
			int expand)
{
	pid_t	pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		write(2, "Fork failed \n", 13);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(delimiter, pipefd, expand);
	else
		return (heredoc_parent(pid, pipefd, filename));
	return (0);
}

int	preprocess_heredocs(t_redirect *redir)
{
	char	*filename;
	int		result;

	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			filename = generate_heredoc_filename();
			result = read_heredoc_to_file(redir->file, filename, \
						redir->delimiter_quoted == 0);
			if (result == -2)
				return (-1);
			if (result == -1)
			{
				perror("heredoc");
				return (-1);
			}
			redir->heredoc_file = filename;
		}
		redir = redir->next;
	}
	return (0);
}

int	preprocess_heredocs_in_ast(t_ast *node)
{
	int	ret;

	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
	{
		ret = preprocess_heredocs(node->u_content.cmd->redirects);
		if (ret == -1)
			return (-1);
		return (0);
	}
	else if (node->type == NODE_PIPE)
	{
		ret = preprocess_heredocs_in_ast(node->left);
		if (ret == -1)
			return (-1);
		ret = preprocess_heredocs_in_ast(node->right);
		if (ret == -1)
			return (-1);
		return (0);
	}
	return (0);
}

void	cleanup_all_heredocs(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_COMMAND && node->u_content.cmd && \
			node->u_content.cmd->redirects)
		cleanup_heredoc_files(node->u_content.cmd->redirects);
	cleanup_all_heredocs(node->left);
	cleanup_all_heredocs(node->right);
}
