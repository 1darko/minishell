/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:59:48 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 11:21:55 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_lstlast(t_lexer *lst)
{
	if (!lst)
		return (0);
	while (lst->next != 0)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_lexer **lst, t_lexer **new)
{
	t_lexer	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = (*new);
	else
	{
		temp = *lst;
		temp = ft_lstlast(temp);
		temp->next = (*new);
		(*new)->prev = temp;
	}
}

void	ft_pipeaddback(t_shell **shell, t_herepipe *new)
{
	t_herepipe	*temp;

	if ((*shell)->pipe == NULL)
	{
		(*shell)->pipe = new;
		return ;
	}
	temp = (*shell)->pipe;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}
