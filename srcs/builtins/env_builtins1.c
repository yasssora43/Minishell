/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:13:58 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/22 13:06:17 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_overflow(unsigned long long res, int sign, int digit)
{
	if (res > (unsigned long long)(LLONG_MAX / 10))
		return (0);
	if (res == (unsigned long long)(LLONG_MAX / 10))
	{
		if ((sign == 1 && digit > 7) || (sign == -1 && digit > 8))
			return (0);
	}
	return (1);
}

const char	*parse_sign(const char *str, int *sign)
{
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

int	parse_digits(const char *str, int sign, long long *out)
{
	unsigned long long	res;
	int					digits;
	int					digit;

	res = 0;
	digits = 0;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (!check_overflow(res, sign, digit))
			return (0);
		res = res * 10 + digit;
		str++;
		digits++;
	}
	while (ft_isspace(*str))
		str++;
	if (*str != '\0' || digits == 0)
		return (0);
	*out = (long long)(res * sign);
	return (1);
}

int	ft_strict_atoll(const char *str, long long *out)
{
	int	sign;

	sign = 1;
	str = parse_sign(str, &sign);
	return (parse_digits(str, sign, out));
}

int	builtin_exit(char **args, int fd)
{
	long long		val;

	(void)fd;
	write(1, "exit\n", 5);
	if (!args[1])
	{
		gc_free_all();
		exit(get_shell_instance()->exit_status);
	}
	if (!args[2])
	{
		if (!ft_strict_atoll(args[1], &val))
			handle_exit_argument_error(args[1], 2);
		gc_free_all();
		exit((unsigned char)val);
	}
	else
	{
		if (ft_strict_atoll(args[1], &val))
			return (handle_too_many_args(args[1]));
		else
			handle_exit_argument_error(args[1], 2);
	}
	return (0);
}
