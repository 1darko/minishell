/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 14:01:49 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/01 14:15:45 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strdup_lex(char *src, int size)
{
	int		i;
	char	*new;

	new = malloc(sizeof(char) * (size + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

static int	add_lex_node(t_lexer **lex, char *str, int *i)
{
	t_lexer	*new;
	int		size;
	int		before;
	int		end;

	size = 0;
	new = malloc(sizeof(*new));
	before = *i;
	if (!new)
		return (1);
	while (str[*i] && (ft_strchr(" \t\n\r\v><|&()", str[*i]) == 0))
	{
		(*i)++;
		size++;
	}
	new->type = LEX_WORD;
	new->next = NULL;
	new->prev = NULL;
	new->heredoc = ft_strdup_lex(str + before, size);
	ft_lstadd_back(lex, &new);
	return (0);
}

int	lexer(t_shell **shell, char *str)
{
	int		i;
	t_lexer	*lexer;

	i = 0;
	lexer = ft_calloc(sizeof(*lexer), 1);
	if (!lexer)
		return (1);
	while (str[i])
	{
		while (str[i] && ft_strchr(" \t\n\r\v", str[i]))
			i++;
		if (str[i] && ft_strchr("><|&()", str[i]))
		{
			if (sub_lexer(&lexer, str, &i))
				return (printf("Error\n"), free_lexer(lexer), 1);
			continue ;
		}
		if (str[i] && !ft_strchr(" \t\n\r\v><|&()", str[i]))
			if (add_lex_node(&lexer, str, &i))
				return (free_lexer(lexer), 1);
	}
	if (lexing_check(shell, lexer))
		return (free_lexer(lexer), free_herepipe(shell), 1);
	return (free_lexer(lexer), 0);
}
