/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:52:57 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 19:33:15 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	cur;
	size_t	count;
	size_t	start;

	cur = 0;
	if (*little == '\0')
		return ((char *) big);
	while (cur < len && big[cur] && (big[cur] != '\0'
			|| little[count] != '\0'))
	{
		count = 0;
		if (big[cur] == little[count])
		{
			start = cur;
			while (big[cur + count] == little[count]
				&& little[count] && cur + count < len)
				count++;
		}
		if (little[count] == '\0')
			return ((char *) big + start);
		cur++;
	}
	return (NULL);
}
// #include <stdio.h>
// int main(void)
// {
// 	char s[30] = "aaabcabcd";
// 	char a[10] = "aabc";
//     printf("%d", ft_strnstr(s, a, -1) == strnstr(s, a, -1));
//     return 0;
// }
