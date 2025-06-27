/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:05:30 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 17:09:26 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_redirections(t_redirect *redir)
{
	return (redir != NULL);
}

int	handle_input_redirection(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open input");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirection(char *file, int append)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		perror("open output");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc_redirection(char *heredoc_file)
{
	int	fd;

	if (!heredoc_file)
	{
		write(2, "heredoc: missing file\n", 22);
		return (-1);
	}
	fd = open(heredoc_file, O_RDONLY);
	if (fd == -1)
	{
		perror("open heredoc");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 heredoc");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redirect *redirects)
{
	while (redirects)
	{
		if (redirects->type == REDIR_IN)
		{
			if (handle_input_redirection(redirects->file) == -1)
				return (-1);
		}
		else if (redirects->type == REDIR_OUT)
		{
			if (handle_output_redirection(redirects->file, 0) == -1)
				return (-1);
		}
		else if (redirects->type == REDIR_APPEND)
		{
			if (handle_output_redirection(redirects->file, 1) == -1)
				return (-1);
		}
		else if (redirects->type == REDIR_HEREDOC)
		{
			if (handle_heredoc_redirection(redirects->heredoc_file) == -1)
				return (-1);
		}
		redirects = redirects->next;
	}
	return (0);
}
