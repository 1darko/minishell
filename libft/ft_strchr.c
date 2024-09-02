/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:50:28 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/21 15:30:44 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;
	int		i;

	i = 0;
	c = c % 256;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			ptr = (char *) s + i;
			return ((char *) ptr);
		}
		i++;
	}
	if (s[i] == c)
		return ((char *) s + i);
	ptr = 0;
	return (ptr);
}
// #include <string.h>
// #include <stdio.h>
// int main ()
// {
//   char str[] = "Bonjoué";
//   printf("%s \n",ft_strchr(str,'é'));
//  printf("%s\n",strchr(str,1));
//   return 0;
// }

// A VOIR POUR LE NULL*/
