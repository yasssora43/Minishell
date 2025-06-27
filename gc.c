/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhindou <ykhindou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:35:58 by ykhindou          #+#    #+#             */
/*   Updated: 2025/05/25 11:39:25 by ykhindou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	*gc_malloc(size_t size)
{
	void		*ptr;
	t_gc_node	*node;
	t_shell		*shell;

	shell = get_shell_instance();
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = shell->gc_list;
	shell->gc_list = node;
	return (ptr);
}

void	*gc_track(void *existing_ptr)
{
	t_gc_node	*node;
	t_shell		*shell;

	shell = get_shell_instance();
	if (!existing_ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
		return (NULL);
	node->ptr = existing_ptr;
	node->next = shell->gc_list;
	shell->gc_list = node;
	return (existing_ptr);
}

void	gc_free_all(void)
{
	t_gc_node	*tmp;
	t_shell		*shell;

	shell = get_shell_instance();
	while (shell->gc_list)
	{
		tmp = shell->gc_list;
		shell->gc_list = shell->gc_list->next;
		if (tmp->ptr)
			free(tmp->ptr);
		if (tmp)
			free(tmp);
	}
}
