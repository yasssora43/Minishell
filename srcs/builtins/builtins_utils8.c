/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:27:26 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/19 21:03:52 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_isnumber(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

char	*ft_strstr(char *str, char *to_find)
{
	int	i;
	int	j;

	if (!str || !to_find)
		return (NULL);
	i = 0;
	j = 0;
	if (to_find[0] == '\0')
		return (str);
	while (str[i])
	{
		while (str[i + j] == to_find[j] && to_find[j] != '\0')
		{
			j++;
		}
		if (to_find[j] == '\0')
		{
			return (&str[i]);
		}
		i++;
		j = 0;
	}
	return (0);
}

void	init_shell_level(t_shell *shell)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = get_env_value2(shell->env, "SHLVL");
	if (!shlvl_str || !ft_isnumber(shlvl_str))
		shlvl = 0;
	else
		shlvl = ft_atoi(shlvl_str);
	if (shlvl < 0)
		shlvl = 0;
	else if (shlvl >= 999)
		shlvl = 1;
	else
		shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	set_env_var("SHLVL", new_shlvl);
}

void	init_default_env(t_shell *shell)
{
	char	buf[PATH_MAX];

	if (!get_env_value2(shell->env, "PWD"))
	{
		if (getcwd(buf, sizeof(buf)))
			set_env_var("PWD", buf);
	}
	if (!get_env_value2(shell->env, "SHLVL"))
		set_env_var("SHLVL", "0");
	init_shell_level(shell);
}
