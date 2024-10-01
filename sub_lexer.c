/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:37:20 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/01 13:43:18 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	*type_setter(int f, int s)
{
	int	*i;

	i = malloc(sizeof(int) * 2);
	i[0] = f;
	i[1] = s;
	return (i);
}

static int	type_return(int *type, char symbol, char *str, int *i)
{
	int	j[2];

	j[0] = type[0];
	j[1] = type[1];
	free(type);
	if (str[*i + 1] == symbol)
	{
		(*i)++;
		return (j[1]);
	}
	return (j[0]);
}

static int	and_return(t_lexer **new, char *str, int *i)
{
	(*new)->type = LEX_AND;
	if (str[++(*i)] == '&')
		;
	else
		return (free(*new), 1);
}

int	sub_lexer(t_lexer **lex, char *str, int *i)
{
	t_lexer	*new;

	new = ft_calloc(sizeof(*new), 1);
	if (!new)
		return (1);
	if (str[*i] == '>')
		new->type = type_return(type_setter(LEX_REDIRIN, \
		LEX_APPEND), '>', str, i);
	else if (str[*i] == '<')
		new->type = type_return(type_setter(LEX_REDIROUT, \
		LEX_HERE), '<', str, i);
	else if (str[*i] == '(')
		new->type = LEX_OPEN;
	else if (str[*i] == ')')
		new->type = LEX_CLOSE;
	else if (str[*i] == '&')
		and_return(new, str, i);
	else if (str[*i] == '|')
		new->type = type_return(type_setter(LEX_PIPE, LEX_OR), '|', str, i);
	(*i)++;
	return (ft_lstadd_back(lex, &new), 0);
}
