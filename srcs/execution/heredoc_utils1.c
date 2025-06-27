/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:31:28 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 18:33:47 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*generate_heredoc_filename(void)
{
	static int	count = 0;
	static int	count2 = 100;
	char		*filename;

	while (1)
	{
		filename = ft_strjoin("heredoc_filename_", ft_itoa(count2++));
		filename = ft_strjoin(filename, ft_itoa(count++));
		if (access(filename, F_OK) == -1)
			return (filename);
	}
}

void	cleanup_heredoc_files(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->heredoc_file)
			unlink(redir->heredoc_file);
		redir = redir->next;
	}
}
