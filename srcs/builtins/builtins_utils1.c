/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:09:46 by ykhindou          #+#    #+#             */
/*   Updated: 2025/06/16 18:19:28 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static char	*find_equal_or_end(char *str)
{
	char	*equal_pos;

	equal_pos = ft_strchr(str, '=');
	if (equal_pos == NULL)
		equal_pos = str + ft_strlen(str);
	return (equal_pos);
}

static int	compare_names(char *str1, char *pivot)
{
	char	*name_end;

	name_end = find_equal_or_end(str1);
	return (ft_strncmp(str1, pivot, name_end - str1));
}

int	partition(char *arr[], int low, int high)
{
	char	*pivot;
	int		i;
	int		j;

	pivot = arr[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (compare_names(arr[j], pivot) < 0)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
		j++;
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void	quick_sort(char *arr[], int low, int high)
{
	int	pi;

	if (low < high)
	{
		pi = partition(arr, low, high);
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
}
