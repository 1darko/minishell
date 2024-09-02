/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:56:52 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/21 18:10:15 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	size_count(long n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		count++;
		n = n * -1;
	}
	while (n > 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static char	*strrev(char *str)
{
	int		end;
	int		i;
	char	temp;

	i = 0;
	if (str[0] == '-')
		i++;
	end = ft_strlen(str) - 1;
	while (i < end)
	{
		temp = str[i];
		str[i] = str[end];
		str[end] = temp;
		i++;
		end--;
	}
	return (str);
}

static char	*itsazero(void)
{
	char	*zero;

	zero = (char *)malloc(sizeof(char) * 2);
	if (!zero)
		return (0);
	zero[0] = '0';
	zero[1] = '\0';
	return (zero);
}

static char	*strcreator(char *str, int i, long int n)
{
	while (n != 0)
	{
		str[i] = (n % 10) + '0';
		i++;
		n = n / 10;
	}
	str[i] = '\0';
	str = strrev(str);
	return (str);
}

char	*ft_itoa(int num)
{
	int				size;
	int				i;
	char			*str;
	long			n;

	if (num == 0)
		return (itsazero());
	i = 0;
	n = num;
	size = size_count(n);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	if (n < 0)
	{
		str[i++] = '-';
		n = n * -1;
	}
	str = strcreator(str, i, n);
	return (str);
}
// int main()
// {   
//     int a = 0;
// 	char *c = ft_itoa(a);
//     printf("%s", c);
// 	free(c);
//     return (0);
// }
