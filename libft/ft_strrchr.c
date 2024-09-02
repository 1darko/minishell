/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:52:42 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/21 15:40:24 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char		*ptr;
	int			i;

	i = 0;
	c %= 256;
	ptr = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			ptr = (char *) s + i;
		i++;
	}
	if (s[i] == c)
		ptr = (char *) s + i;
	return ((char *)ptr);
}
// #include <stdio.h>
// int main ()
// {
//   char str[] = "";
//   printf("%s \n",ft_strrchr(str,'\0'));
//   printf("%s \n",strrchr(str,'\0'));
//   return 0;
// }
