/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   han_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:38:58 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/21 10:55:47 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_dollar_cases(char *tgt, char **env, t_bool f, int *i)
{
	char	*out;
	char	tmp[3];

	out = ft_strdup("");
	if (tgt[*i + 1] == '?')
	{
		out = ft_strjoin(out, exit_status(i));
		(*i) += 2;
	}
	else if (ft_isdigit(tgt[*i + 1]))
		out = ft_strjoin(out, digit(tgt, i));
	else if (iscaracter(tgt[*i + 1]))
	{
		tmp[0] = '$';
		tmp[1] = tgt[*i + 1];
		tmp[2] = '\0';
		out = ft_strjoin(out, ft_strdup(tmp));
		(*i) += 2;
	}
	else if (!ft_isalnum(tgt[*i + 1]) && tgt[*i + 1] != '_')
		out = ft_strjoin(out, special_cases(tgt, i));
	else
		out = ft_strjoin(out, var_expand(tgt, env, f, i));
	return (out);
}

static char	*handle_dollar_expansion(char *tgt, char **env, t_bool f, int *i)
{
	char	*out;

	out = ft_strdup("");
	if (!tgt[*i + 1])
	{
		out = ft_strjoin(out, ft_strdup("$"));
		(*i)++;
		return (out);
	}
	out = handle_dollar_cases(tgt, env, f, i);
	return (out);
}

char	*expand_token(char *target, char **env, t_bool flag)
{
	char	*output;
	char	*temp;
	int		i;
	int		in_single;
	int		in_double;

	output = ft_strdup("");
	i = 0;
	in_single = 0;
	in_double = 0;
	while (target[i])
	{
		handle_quotes_state(target[i], &in_single, &in_double);
		if (target[i] == '$' && !in_single)
			output = ft_strjoin(output,
					handle_dollar_expansion(target, env, flag, &i));
		else
		{
			temp = ft_substr(target, i, 1);
			output = ft_strjoin(output, temp);
			i++;
		}
	}
	return (output);
}

char	*handle_env_expansion(
	char *target, char **env, t_bool flag, int *i)
{
	char	*output;

	output = ft_strdup("");
	if (target[*i + 1] == '?')
		output = ft_strjoin(output, exit_status(i));
	else if (!ft_isalnum(target[*i + 1]) && target[*i + 1] != '_')
		output = ft_strjoin(output, special_cases(target, i));
	else if (ft_isdigit(target[*i + 1]))
		output = ft_strjoin(output, digit(target, i));
	else
		output = ft_strjoin(output, var_expand(target, env, flag, i));
	return (output);
}

char	*get_env_in_herdoc(char *target, char **env, t_bool flag, char *del)
{
	char	*output;
	int		i;

	output = ft_strdup("");
	i = 0;
	while (target[i])
	{
		if (target[i] == '$' && !not_expanded(del))
			output = ft_strjoin(output,
					handle_env_expansion(target, env, flag, &i));
		else
			output = ft_strjoin(output, ft_copy(target, &i));
	}
	return (output);
}
