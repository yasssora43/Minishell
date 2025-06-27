/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:59:44 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 13:05:38 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i])
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = s2[i];
	return (s1);
}

char	*ft_strcat(char *dest, const char *src)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	while (dest[i] != '\0')
		i++;
	j = 0;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*get_env_value2(char **env, const char *key)
{
	int		i;
	size_t	len;

	if (!env || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}

void	handle_exit_argument_error(char *arg, int code)
{
	printf("exit: %s: numeric argument required\n", arg);
	gc_free_all();
	exit(code);
}

int	handle_too_many_args(char *arg)
{
	printf("exit: %s: too many arguments\n", arg);
	return (1);
}
