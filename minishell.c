/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:23:38 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/19 20:34:12 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

char	**dup_env(char **env)
{
	char	**copy;
	int		count;
	int		i;

	count = 0;
	while (env[count])
		count++;
	copy = (char **)gc_malloc((count + 1) * sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		if (!copy[i])
		{
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_shell	*get_shell_instance(void)
{
	static t_shell	shell;

	return (&shell);
}

void	signal_handler(int sig)
{
	printf("signal %d is received \n.", sig);
}

void	minishell_loop(t_shell *shell)
{
	char	*line;
	t_ast	*ast;

	while (shell->running)
	{
		setup_signals();
		line = readline("minishell $ ");
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		gc_track(line);
		add_history(line);
		ast = parser(line);
		if (ast)
			shell->exit_status = execute_shell(ast, shell);
		cleanup_all_heredocs(ast);
	}
	clear_history();
}

int	main(void)
{
	t_shell		*shell;
	extern char	**environ;

	shell = get_shell_instance();
	shell->gc_list = NULL;
	shell->env = dup_env(environ);
	init_default_env(shell);
	shell->exit_status = 0;
	shell->running = 1;
	minishell_loop(shell);
	gc_free_all();
	return (shell->exit_status);
}
