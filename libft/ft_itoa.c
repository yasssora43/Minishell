/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 19:17:02 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/19 14:17:57 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_size(long n)
{
	int	size;

	if (n == 0)
		return (1);
	size = 0;
	if (n < 0)
		size++;
	while (n)
	{
		size++;
		n = n / 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*digits;
	int		len;
	long	m;

	m = n;
	len = count_size(m);
	digits = (char *)gc_malloc((len + 1) * sizeof(char));
	if (!digits)
		return (NULL);
	digits[len--] = '\0';
	if (m == 0)
		digits[0] = '0';
	if (m < 0)
	{
		digits[0] = '-';
		m *= -1;
	}
	while (m)
	{
		digits[len--] = (m % 10) + '0';
		m /= 10;
	}
	return (digits);
}
