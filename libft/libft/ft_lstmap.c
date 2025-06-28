/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgrandia <mgrandia@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:24:37 by mgrandia          #+#    #+#             */
/*   Updated: 2025/05/10 18:38:34 by mgrandia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_element;
	t_list	*new_node;

	new_list = NULL;
	while (lst)
	{
		new_element = f(lst -> content);
		if (!new_element)
		{
			return (NULL);
		}
		new_node = ft_lstnew(new_element);
		if (!new_node)
			return (NULL);
		ft_lstadd_back(&new_list, new_node);
		lst = lst -> next;
	}
	if (lst)
	{
		free(new_element);
		ft_lstclear(&new_list, del);
		return (NULL);
	}
	return (new_list);
}
