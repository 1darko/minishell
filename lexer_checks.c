/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:35:55 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/03 10:36:02 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int prev_check(t_lexer *lex, char *str)
{
    int i;

    i = 0;
    if(!lex)
        return (1);
    while(str[i])
    {   
        if((lex->prev->type + 48) == str[i])
            return (0);
        if(str[i] == '>' && lex->prev->type == LEX_APPEND)
            return (0);
        i++;
    }
    return (1);
}
int next_check(t_lexer *lex, char *str)
{
    int i;

    i = 0;
    if(!lex)
        return (1);
    while(str[i])
    {
        if(lex->next == 0)
        {
            if(str[i] == '0')
                return (0);
            else
                return (1);
        }
        if((lex->next->type + 48) == str[i])
            return (0);
        i++;
    }
    return (1);
}
int first_type_check(t_lexer *lex, int *i, t_shell **shell)
{
    if(lex->type != LEX_START)
    {
        printf("Minishell: syntax error near unexpected token `newline\'\n");
        return (1);
    }
    return (0);
}