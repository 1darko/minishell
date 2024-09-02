/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:51:39 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/10 11:14:51 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(const char *s, int fd)
{
	if (!fd || !s)
		return ;
	while (*s)
		write (fd, &(*(s++)), sizeof(char));
}
// int main(void)
// {
//     ft_putstr_fd("BONJOUR  _",1);
//     return 0;
// }
