/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 12:18:22 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/17 15:15:31 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return ;
}

void	free_lexer(t_lexer *lexer)
{
	t_lexer	*lex;

	while (lexer)
	{
		lex = lexer->next;
		if (lexer->heredoc)
			free(lexer->heredoc);
		free(lexer);
		lexer = lex;
	}
}

void	tree_free(t_cmd **tree)
{
	t_execcmd	*ex;
	t_redircmd	*re;
	t_doublecmd	*db;
	t_sub		*sub;

	if (tree == NULL || *tree == NULL)
		return ;
	if ((*tree)->type == REDIR)
		free_redir((t_redircmd *)(*tree));
	else if ((*tree)->type == EXEC)
		free_exec((t_execcmd *)(*tree));
	else if ((*tree)->type == SUB)
		free_sub((t_sub *)(*tree));
	else
		free_double((t_doublecmd *)(*tree));
}

void	free_herepipe(t_shell **shell)
{
	t_herepipe	*temp;

	temp = (*shell)->pipe;
	while ((*shell)->pipe)
	{
		temp = (*shell)->pipe->next;
		// Access the string before freeing
		// Free the memory after you have printed or used the string
		free((*shell)->pipe->str);
		free((*shell)->pipe);
		// printf("PIPE EST A %s\n", (*shell)->pipe->str);

		(*shell)->pipe = temp;
	}
	return ;
}
