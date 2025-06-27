/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:30:57 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 13:01:50 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	write_line_to_pipe(char *line, int fd, int expand)
{
	char	*expanded;

	gc_track(line);
	if (expand)
	{
		expanded = expand_variables(line);
		line = expanded;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	print_eof_warning(const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by \
                    end-of-file (wanted `", 2);
	ft_putstr_fd((char *)delimiter, 2);
	ft_putendl_fd("')", 2);
}

void	heredoc_child(const char *delimiter, int *pipefd, int expand)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			print_eof_warning(delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		write_line_to_pipe(line, pipefd[1], expand);
	}
	close(pipefd[1]);
	gc_free_all();
	exit(0);
}
