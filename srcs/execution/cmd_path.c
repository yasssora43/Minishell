/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:26:55 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 15:21:53 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_variable(const char *name, char **env)
{
	size_t	name_len;
	int		i;

	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(get_shell_instance()->env[i], name, name_len) == 0 && \
				env[i][name_len] == '=')
			return (env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

char	*build_full_path(const char *dir, const char *cmd)
{
	size_t	len;
	char	*full_path;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = gc_malloc(len);
	if (!full_path)
		return (NULL);
	ft_strcpy(full_path, (char *)dir);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, cmd);
	return (full_path);
}

char	*find_command_in_paths(char **paths, const char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	full_path = NULL;
	while (paths[i])
	{
		full_path = build_full_path(paths[i], cmd);
		if (!full_path)
			continue ;
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, char **env)
{
	char	*path_env;
	char	*result;
	char	**dirs;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		perror("access failed");
		return (NULL);
	}
	path_env = get_env_variable("PATH", env);
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	result = find_command_in_paths(dirs, cmd);
	return (result);
}
