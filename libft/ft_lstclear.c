/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:53:37 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/09 16:09:28 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* A CHANGER IL NE DEL PAS TOUT MAIS UN SEUL CONTENT*/
void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	if (!del || !lst || !(*lst))
		return ;
	ft_lstclear (((&(*lst)->next)), del);
	(del)((*lst)->content);
	free(*lst);
	*lst = NULL;
}
