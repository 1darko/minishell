/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:20:17 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 11:22:09 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*s3;
	unsigned char	*s4;

	s3 = (unsigned char *)s1;
	s4 = (unsigned char *)s2;
	while (*s3 == *s4 && *s3 != '\0')
	{
		++s3;
		++s4;
	}
	return (*s3 - *s4);
}

char	*ft_strchr(char *s, char c)
{
	while (*s)
	{
		if (*s == c)
			return (s);
		s++;
	}
	return (0);
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

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

void	ft_strcpy_quoteless(char **dest, char *src)
{
	char	*temp;

	temp = *dest;
	while (*src)
	{
		if (*src != '\'' && *src != '\"')
		{
			*temp = *src;
			++temp;
		}
		++src;
	}
	*temp = '\0';
}
