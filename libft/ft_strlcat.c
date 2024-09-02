/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:51:02 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/11 12:22:51 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t given_size)
{
	size_t	dest_size;
	size_t	src_size;
	size_t	i;

	i = 0;
	if (given_size == 0)
		return (ft_strlen(src));
	dest_size = ft_strlen(dest);
	src_size = ft_strlen(src);
	if (dest_size > given_size)
		src_size = given_size + src_size;
	else
		src_size = src_size + dest_size;
	while (dest_size + i < given_size - 1 && src[i] != '\0')
	{
		dest[dest_size + i] = src[i];
		i++;
	}
	dest[dest_size + i] = '\0';
	return (src_size);
}
/*  These functions copy and catenate the input string into a 
destination string.  If the destination buffer, limited by its  size,
isn't  large  enough  to  hold the copy, the resulting string is
truncated (but it is guaranteed to  be  null-terminated).   They
return  the  length  of  the  total string they tried to create.
These functions force a SIGSEGV if the  src  pointer  is  not  a
string.
*/
/*
int main(void)
{   
    char a[55] = "";
    const char b[6] = "ABCDe";
    char c[55] = "";
    const char d[6] = "ABCDe";

    size_t i = ft_strlcat(a, b, 15);
    printf("%zu \n", i);
    printf("%s \n", a);

    return 0;
}*/
