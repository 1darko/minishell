/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:53:14 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 17:49:52 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	t_list	*new_node;
	t_list	*copy;

	if (!f || !del || !lst)
		return (NULL);
	copy = NULL;
	while (lst != 0)
	{
		temp = f(lst->content);
		new_node = ft_lstnew(temp);
		if (!new_node)
		{
			del(temp);
			ft_lstclear(&copy, del);
			return (copy);
		}
		ft_lstadd_back(&copy, new_node);
		lst = lst->next;
	}
	return (copy);
}
	// while (lst != 0)
	// {
	// 	new = malloc(sizeof(t_list));
	// 	if (!new)
	// 	{
	// 		del(copy);
	// 		return (NULL);
	// 	}
	// 	new = ft_lstnew(f(lst->content));
	// 	if (copy == 0)
	// 		copy = new;
	// 	else
	// 	{
	// 		temp = copy;
	// 		temp = ft_lstlast(temp);
	// 		temp->next = new;
	// 	}			
	// 	lst = lst->next;
	// }	