/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:39:09 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 11:35:58 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/minishell.h"		

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*alloc;

	if (nmemb != 0 && (size > SIZE_MAX / nmemb))
		return (NULL);
	alloc = gc_malloc(nmemb * size);
	if (alloc == NULL)
		return (NULL);
	ft_bzero(alloc, nmemb * size);
	return (alloc);
}
