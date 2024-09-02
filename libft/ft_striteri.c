/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:50:45 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/09 13:50:53 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*function)(unsigned int, char *))
{
	unsigned int	cur;

	cur = 0;
	if (s && function)
	{
		while (s[cur] != '\0')
		{
			function(cur, &s[cur]);
			cur++;
		}
	}
}
