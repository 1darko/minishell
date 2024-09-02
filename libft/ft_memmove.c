/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:54:00 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 17:51:45 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*tdest;
	unsigned char	*tsrc;
	size_t			cur;

	tdest = (unsigned char *)dest;
	tsrc = (unsigned char *)src;
	if ((!dest && !src) || !n)
		return (dest);
	cur = 0;
	if (tdest <= tsrc)
	{
		while (cur < n)
		{
			tdest[cur] = tsrc[cur];
			cur++;
		}
	}
	else
		while (n--)
			tdest[n] = tsrc[n];
	return (dest);
}
