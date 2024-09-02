/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:50:10 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/09 13:50:11 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*adr;
	size_t			cur;

	cur = 0;
	adr = (unsigned char *)s;
	while (cur < n)
	{
		if (adr[cur] == (unsigned char) c)
			return (adr + cur);
		cur++;
	}
	return (NULL);
}

/*int main(void)
{
	char src[50] = "mahmudulhasanjony";
	char dest[50];
	char c = 'n';
	char *ret;

	printf("%s",ft_memchr(src, c, 25));
	
	return (0);
}*/
