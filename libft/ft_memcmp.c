/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:49:31 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/10 11:00:27 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*c1;
	unsigned char		*c2;
	size_t				cur;

	if (n <= 0)
		return (0);
	cur = 0;
	c1 = (unsigned char *) s1;
	c2 = (unsigned char *) s2;
	while (cur < n)
	{
		if (c1[cur] != c2[cur])
			return (c1[cur] - c2[cur]);
		cur++;
	}
	return (0);
}
// #include <string.h>
// int main(void)
// {
//     const char s1[50] = "Bonc";
//     const char s2[50] = "Bonc";
//     const char t1[50] = "Bonc";
//     const char t2[50] = "Bonc";
//     printf("%d \n", ft_memcmp(s1, s2, 100000));
//     printf("%d", memcmp(s1, s2, 100000));
//     return 0;
// }
