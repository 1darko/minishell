/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:53:06 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 17:52:33 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char const *s, int fd)
{
	if (!s || !fd)
		return ;
	while (*s != '\0')
		ft_putchar_fd (*(s++), fd);
	ft_putchar_fd('\n', fd);
}
// int main(void)
// {   
//     ft_putendl_fd("Bonjour.", 1);
//     return 0;
// }
