/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 13:56:35 by dakojic           #+#    #+#             */
/*   Updated: 2023/11/20 19:48:15 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_char(const char *set, char c)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

static char	*strmalloc(int *tab)
{
	char	*new;

	if (tab[1] == 0)
		new = (char *)malloc(sizeof(char) * (tab[0]));
	else
		new = (char *)malloc(sizeof(char) * (tab[1] - tab[0] + 2));
	if (!new)
		return (0);
	return (new);
}

/*
	int		0	cur;
	int		1	end;
	int		2	new_count;
	*/
char	*ft_strtrim(char const *s1, char const *set)
{
	char		*new;
	int			tab[3];

	tab[2] = 0;
	tab[0] = 0;
	if (!s1 || !set)
		return (0);
	while (check_char(set, s1[tab[0]]) && s1[tab[0]])
		tab[0]++;
	tab[1] = ft_strlen(s1) - 1;
	if (s1[0] == '\0' || tab[1] + 1 == tab[0])
		return (ft_calloc(sizeof(char), 1));
	while (check_char(set, s1[tab[1]]) && tab[1] > 0)
		tab[1]--;
	new = strmalloc(tab);
	if (!new)
		return (NULL);
	while (tab[0] <= tab[1])
		new[tab[2]++] = s1[tab[0]++];
	new[tab[2]] = 0;
	return (new);
}
/*#include <stdio.h>
 int main(void)
 {
     char a[50] = "123132aaacxabbxccccx654123111";
     char b[4] = "123";
     char *c = ft_strtrim(a,b);
     printf("%s", c);
     return 0;
 }*/
