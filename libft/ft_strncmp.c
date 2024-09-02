/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:52:04 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/09 13:52:05 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s3;
	unsigned char	*s4;
	size_t			cur;

	cur = 0;
	s3 = (unsigned char *) s1;
	s4 = (unsigned char *) s2;
	if (n == 0)
		return (0);
	while (cur < n && s3[cur] != '\0' && s4[cur] != '\0')
	{
		if (s3[cur] != s4[cur])
			return (s3[cur] - s4[cur]);
		cur++;
	}
	if (cur == n)
		return (0);
	return (s3[cur] - s4[cur]);
}
// #include <stdio.h>
// int main(void)
// {
//     const char* a = "Benjuzeaze";
//     const char* b = "Bonjour";
//     size_t n = 2;
//     printf("%d\n", ft_strncmp(a,b,n));
//     printf("%d\n", strncmp(a,b,n));
//     return 0;
// }
