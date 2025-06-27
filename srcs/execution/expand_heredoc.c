/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:50:31 by yassin            #+#    #+#             */
/*   Updated: 2025/05/29 17:28:40 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_status_code(t_expand_ctx *ctx)
{
	char	*status;

	status = ft_itoa(ctx->shell_instance->exit_status);
	append_str(ctx->expanded_line, status, &ctx->position_in_result);
}

static void	handle_variable(const char *input, t_expand_ctx *ctx)
{
	size_t	start;
	char	*key;
	char	*value;

	start = ctx->position_in_input;
	while (is_var_char(input[ctx->position_in_input]))
		ctx->position_in_input++;
	key = ft_strndup(input + start, ctx->position_in_input - start);
	value = get_key_value(key, ctx->shell_instance->env);
	if (value)
		append_str(ctx->expanded_line, value, &ctx->position_in_result);
}

static int	handle_single_quote(const char *input, t_expand_ctx *ctx)
{
	if (input[ctx->position_in_input] == '\'' && !ctx->inside_single_quote)
	{
		ctx->inside_single_quote = 1;
		ctx->expanded_line[ctx->position_in_result++] = \
		input[ctx->position_in_input++];
		return (1);
	}
	else if (input[ctx->position_in_input] == '\'' && ctx->inside_single_quote)
	{
		ctx->inside_single_quote = 0;
		ctx->expanded_line[ctx->position_in_result++] = \
		input[ctx->position_in_input++];
		return (1);
	}
	return (0);
}

static void	expand_loop_body(const char *input, t_expand_ctx *ctx)
{
	if (handle_single_quote(input, ctx))
		return ;
	if (input[ctx->position_in_input] == '$' && !ctx->inside_single_quote)
	{
		ctx->position_in_input++;
		if (input[ctx->position_in_input] == '?')
		{
			handle_status_code(ctx);
			ctx->position_in_input++;
		}
		else if (is_var_char(input[ctx->position_in_input]))
		{
			handle_variable(input, ctx);
		}
		else
		{
			ctx->expanded_line[ctx->position_in_result++] = '$';
		}
	}
	else
	{
		ctx->expanded_line[ctx->position_in_result++] = \
			input[ctx->position_in_input++];
	}
}

char	*expand_variables(const char *line)
{
	t_expand_ctx	ctx;

	ctx.shell_instance = get_shell_instance();
	ctx.position_in_input = 0;
	ctx.position_in_result = 0;
	ctx.inside_single_quote = 0;
	ctx.expanded_line = gc_malloc(strlen(line) * 1024 + 1);
	if (!ctx.expanded_line)
		return (NULL);
	while (line[ctx.position_in_input])
		expand_loop_body(line, &ctx);
	ctx.expanded_line[ctx.position_in_result] = '\0';
	return (ctx.expanded_line);
}
