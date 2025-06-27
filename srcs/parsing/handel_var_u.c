/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_var_u.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:38:43 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/20 20:22:45 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*digit(char *target, int *i)
{
	char	*result;
	int		jump;
	char	*var;

	result = ft_strdup("");
	jump = *i;
	if (target[jump + 1] == '0')
	{
		result = ft_strdup("Minishell");
		jump += ft_strlen(until_dollar(&target[jump])) + 2;
	}
	else
	{
		var = until_dollar(&target[jump + 2]);
		result = ft_strdup(var);
		jump += ft_strlen(var) + 2;
	}
	*i = jump;
	return (result);
}

char	*special_cases(char *target, int *i)
{
	char	*output;
	char	*variable;

	output = "";
	if (target[(*i)] == '$' && (target[(*i) + 1] == '\"'
			|| target[(*i) + 1] == '\''))
		(*i)++;
	variable = until_dollar(&target[(*i)]);
	output = ft_strjoin(output, variable);
	(*i) += ft_strlen(variable);
	return (output);
}

char	*get_env_value(const char *name, char **env)
{
	int		i;
	size_t	name_len;

	i = 0;
	if (!name || !env || !*name)
		return (ft_strdup(""));
	name_len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0
			&& env[i][name_len] == '=')
			return (ft_strdup(env[i] + name_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char	*var_expand(char *target, char **env, t_bool flag, int *i)
{
	char	*output;
	char	*var_name;
	char	*env_value;
	char	*tmp;
	int		name_len;

	output = ft_strdup("");
	var_name = extract_var_name(target + *i + 1);
	name_len = validate_var_name(target + *i + 1);
	env_value = get_env_value(var_name, env);
	if (!env_value || ft_strlen(env_value) == 0)
	{
		if (flag == TRUE)
			tmp = ft_strjoin(output, "");
		else
			tmp = ft_strjoin(output, "");
	}
	else
		tmp = ft_strjoin(output, env_value);
	*i += name_len + 1;
	return (tmp);
}

char	*exit_status(int *i)
{
	t_shell	*shell;

	shell = get_shell_instance();
	*i += 2;
	return (ft_itoa(shell->exit_status));
}
