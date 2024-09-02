/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:51:48 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 18:27:31 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	i;

	if (n < 0)
	{
		ft_putchar_fd ('-', fd);
		n *= -1;
	}
	i = n;
	if (i > 9)
		ft_putnbr_fd (i / 10, fd);
	ft_putchar_fd (i % 10 + '0', fd);
}
// int main(void)
// {
// ft_putnbr_fd(-5, 2);
//     return 0;
// }
