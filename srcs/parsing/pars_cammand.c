/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_cammand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moelgham <moelgham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:39:12 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/21 14:31:26 by moelgham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool	err(char *prefix, char *token, char *suffix)
{
	ft_putstr_fd(prefix, 2);
	if (token)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("`", 2);
	}
	ft_putstr_fd(suffix, 2);
	return (TRUE);
}

static t_bool	is_only_quotes_or_whitespace(const char *str)
{
	int		quote;
	t_bool	has_content;

	quote = 0;
	has_content = FALSE;
	while (*str)
	{
		if ((*str == '"' || *str == '\'') && (!quote || quote == *str))
		{
			if (quote)
				quote = 0;
			else
				quote = *str;
		}
		else if (!ft_isspace(*str))
			has_content = TRUE;
		str++;
	}
	return (!has_content && !quote);
}

t_bool	enhanced_parse_check(t_token **tokens, size_t count, char *input)
{
	t_shell	*shell;

	if (!input)
		return (FALSE);
	shell = get_shell_instance();
	if (!*input || is_only_quotes_or_whitespace(input))
	{
		ft_putstr_fd("minishell: syntax error: empty input\n", 2);
		shell->exit_status = 2;
		return (TRUE);
	}
	if (validate_initial_tokens(tokens, count) || validate_token(tokens, count)
		|| has_redirection_error(tokens, count))
	{
		shell->exit_status = 2;
		return (TRUE);
	}
	if (ft_strlen(input) == 1 || input[0] == '-')
	{
		ft_putstr_fd("minishell: command not found\n", 2);
		shell->exit_status = 127;
		return (TRUE);
	}
	shell->exit_status = 0;
	return (FALSE);
}
