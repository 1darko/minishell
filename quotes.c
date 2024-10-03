/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:20:28 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/03 11:47:30 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	quote_skipper(char **s, char end)
{
	char	*str;

	str = (*s);
	++str;
	while (*str && *str != (char)end)
		++str;
	if (*str == '\0')
		return (1);
	*s = str;
	return (0);
}

int	quote_check(char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
			if (quote_skipper(&str, *str))
				return (1);
		++str;
	}
	return (0);
}
