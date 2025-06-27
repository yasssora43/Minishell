/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_function_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:35:44 by moelgham          #+#    #+#             */
/*   Updated: 2025/06/19 13:49:08 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_bool	is_consecutive_operators(t_token *cur, t_token *next)
{
	return ((cur->type >= TOK_PIPE && cur->type <= TOK_OR)
		&& (next->type >= TOK_PIPE && next->type <= TOK_OR));
}

t_bool	iscaracter(char c)
{
	if (c == '$' || c == '#' || c == '%' || c == '*'
		|| c == '@' || c == '&' || c == '!' || c == '^'
		|| c == '\n' || c == '/' || c == '[' || c == ']'
		|| c == '|' || c == '}' || c == '{' || c == '('
		|| c == ')' || c == '~' || c == '.' || c == '\\' || c == '+'
		|| c == '=' || c == ',' || c == '`' || c == '-')
		return (TRUE);
	return (FALSE);
}

int	not_expanded(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == S_Q || str[i] == D_Q)
			return (1);
		i++;
	}
	return (0);
}
