/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:51:07 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/21 16:50:51 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen (src);
	if (size <= 0 || !(dst))
		return (len);
	while (i < len && i < size - 1)
		*dst++ = src[i++];
	*dst = 0;
	return (len);
}
// #include <bsd/stdio.h>
// #include <stddef.h>
// int main(void)
// {
// 	char src[] = "coucou";
// 	char dest[10]; memset(dest, 'A', 10);
// 	ft_strlcpy(dest, src, 0);
// 	ft_strlcpy(dest, src, 1);
// 	ft_strlcpy(dest, src, 2);
// 	// ft_strlcpy(dest, src, -1);
// 	// char c[] = "coucou";
// 	// char d[10]; memset(d, 'A', 10);
//     // strlcpy(d, c, 4);
//     printf("%s\n", dest);
// 	printf("%s\n", dest + 2);
//     // printf("%s\n", d);
//     return 0;
// }
