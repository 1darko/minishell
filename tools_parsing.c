/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:19:07 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/26 13:19:24 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lfsymbol(char **ptr, char *c)
{
	char	*temp;

	temp = *ptr;
	while (*temp && ft_strchr(" \t\n\r\v", *temp))
		temp++;
	*ptr = temp;
	return (*ptr && ft_strchr(c, *temp));
}
