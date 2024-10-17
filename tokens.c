/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:03:20 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/17 11:17:21 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*other_token(char *temp, int *check)
{
	char	a;

	a = *temp;
	if (!*temp)
		*check = 0;
	else
		*check = 'a';
	if (a == '\"' || a == '\'')
	{
		temp++;
		while (temp && *temp != a)
			temp++;
		temp++;
		if (*temp == '\'' || *temp == '\"')
			other_token(temp, check);
	}
	else
		while (*temp && !ft_strchr(" \t\n\r\v<>()|&", *temp))
			temp++;
	return (temp);
}

static void	extratoken(char **temp, int *check)
{
	if (*check == '|' && *(*temp) == '|')
	{
		(*temp)++;
		*check = 'o';
	}
	else if (*check == '&' && *(*temp) == '&')
	{
		(*temp)++;
		*check = 'a';
	}
	else if (*check == '>' && *(*temp) == '>')
	{
		(*temp)++;
		*check = '+';
	}
	else
	{
		(*temp)++;
		*check = 'h';
	}
}


int	gettoken(char **ptr, char **ptr_token, char **ptr_endtoken)
{
	char	*temp;
	int		check;

	temp = *ptr;
	while (*temp && ft_strchr(" \t\n\r\v", *temp))
		temp++;
	if (ptr_token)
		*ptr_token = temp;
	check = *temp;
	if (ft_strchr("<>()|&", check))
	{
		temp++;
		if (check == *temp && check != '(' && check != ')')
			extratoken(&temp, &check);
	}
	else
		temp = other_token(temp, &check);
	if (ptr_endtoken)
		*ptr_endtoken = temp;
	*ptr = temp;
	return (check);
}