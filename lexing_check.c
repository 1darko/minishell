/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:27:57 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/01 14:17:03 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	type_check3(t_lexer *lex, int *i)
{
	if (lex->type == LEX_PIPE)
	{
		if (prev_check(lex, "17"))
			return (printf("Minishell: syntax error near\
unexpected token `|\'\n"), 1);
	}
	else if (lex->type == LEX_OPEN && ++(*i))
	{
		if (prev_check(lex, "-04568"))
		{
			return (printf("Minishell: syntax error near\
unexpected token `(\'\n"), 1);
		}
	}
	else if (lex->type == LEX_CLOSE && (*i)-- >= -1)
	{
		if (prev_check(lex, "17"))
			return (printf("Minishell: syntax error near\
unexpected token `)\'\n"), 1);
	}
	return (0);
}

static int	type_check2(t_lexer *lex, int *i)
{
	if (lex->type == LEX_WORD)
	{
		if (prev_check(lex, "012345689>"))
			return (printf("Lexer : syntax error\n"), 1);
	}
	else if (lex->type == LEX_REDIROUT)
	{
		if (prev_check(lex, "-0145678"))
			return (printf("Minishell: syntax error near\
unexpected token `<'\n"), 1);
	}
	else if (lex->type == LEX_REDIRIN)
	{
		if (prev_check(lex, "-0145678"))
			return (printf("Minishell: syntax\
error near unexpected token `>'\n"), 1);
	}
	else if (lex->type == LEX_AND)
	{
		if (prev_check(lex, "17"))
			return (printf("Minishell:  syntax error near\
unexpected token `&&\'\n"), 1);
	}
	return (0);
}

static int	type_check(t_lexer *lex, int *i, t_shell **shell)
{
	if (type_check2(lex, i) || type_check3(lex, i))
		return (1);
	else if (lex->type == LEX_OR)
	{
		if (prev_check(lex, "17"))
			return (printf("Minishell: syntax error \
near unexpected token ||\n"), 1);
	}
	else if (lex->type == LEX_HERE)
	{
		if (prev_check(lex, "0145678") || next_check(lex, "1"))
			return (printf("Minishell: syntax error near \
unexpected token `<<\'\n"), 1);
		if (init_heredoc(lex, shell))
			return (1);
	}
	else if (lex->type == LEX_APPEND)
	{
		if (prev_check(lex, "-0145678"))
			return (printf("Minishell: syntax error near \
unexpected token `>>'\n"), 1);
	}
	return (0);
}

static int	last_type_check(int lex)
{
	if (lex != 1 && lex != 7)
	{
		printf("Minishell: syntax error near unexpected token `newline\'\n");
		return (1);
	}
	return (0);
}

int	lexing_check(t_shell **shell, t_lexer *lexer)
{
	t_lexer		*lex;
	int			i;

	i = 0;
	lex = lexer;
	if (first_type_check(lex, &i, shell))
		return (1);
	lex = lex->next;
	while (lex)
	{
		if (type_check(lex, &i, shell))
			return (1);
		if (lex->next == NULL)
			if (last_type_check(lex->type))
				return (1);
		lex = lex->next;
		if (i < 0)
			return (printf("Minishell: syntax error\
			 near unexpected token `)\'\n"), 1);
	}
	if (i)
		return (printf("Minishell: syntax error \
		near unexpected token `(\'\n"), 1);
	return (0);
}
