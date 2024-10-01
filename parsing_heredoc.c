/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_heredoc                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:24:44 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 11:25:15 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*quote_remove(int *quote, char *str)
{
	int		i;
	char	*ret;
	char	*temp;

	temp = str;
	i = 0;
	while (*temp)
	{
		if (*temp != '\'' && *temp != '\"')
			i++;
		else
			(*quote)++;
		temp++;
	}
	ret = malloc(sizeof(char) * (1 + i));
	if (!ret)
		return (NULL);
	ft_strcpy_quoteless(&ret, str);
	return (ret);
}

int	init_heredoc(t_lexer *lex, t_shell **shell)
{
	char		*eof;
	char		*temp;
	t_herepipe	*node;

	node = malloc(sizeof(*node));
	node->quote = 0;
	temp = quote_remove(&node->quote, lex->next->heredoc);
	if (!temp)
		return (free(node), 1);
	node->str = heredoc_filler(temp);
	free(temp);
	if (!node->str)
		return (1);
	node->stored = 0;
	node->next = NULL;
	ft_pipeaddback(shell, node);
	return (0);
}
