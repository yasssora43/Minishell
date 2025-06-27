/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_qoutes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:38:28 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/21 11:45:39 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_quoted_segment(char *str, char *new_str, int *i, int *j)
{
	char	quote;
	int		k;

	quote = str[*i];
	k = *i + 1;
	while (str[k] && str[k] != quote)
		k++;
	if (str[k] == quote)
	{
		(*i)++;
		while (str[*i] && str[*i] != quote)
			new_str[(*j)++] = str[(*i)++];
		(*i)++;
	}
	else
		new_str[(*j)++] = str[(*i)++];
	return (0);
}

static int	copy_without_quotes(char *str, char *new_str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == S_Q || str[i] == D_Q)
			handle_quoted_segment(str, new_str, &i, &j);
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (0);
}

char	*remove_quotes(char *str)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = gc_malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	copy_without_quotes(str, new_str);
	return (new_str);
}

t_bool	check_unclosed_quotes(char *str)
{
	int		i;
	int		in_single;
	int		in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (str && str[i])
	{
		if (str[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (str[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	if (in_single || in_double)
	{
		ft_putstr_fd("minishell: unclosed quotes\n", 2);
		get_shell_instance()->exit_status = 258;
		return (TRUE);
	}
	return (FALSE);
}

void	handle_quotes_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}
