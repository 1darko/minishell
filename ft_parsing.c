/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:11:45 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 14:40:32 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*parsecmd(t_shell **shell, char *str)
{
	t_cmd	*cmdtree;

	if (str[0] == '\0')
		return (free(str), NULL);
	if (lexer(shell, str))
		return (NULL);
	(*shell)->tree = parseline(*shell, &str);
	return (0);
}

t_cmd	*parseline(t_shell *shell, char **ptr)
{
	t_cmd	*cmd;

	cmd = parse_double_node(shell, ptr);
	return (cmd);
}

t_cmd	*parse_double_node(t_shell *shell, char **ptr)
{
	t_cmd		*cmd;
	char		token;

	cmd = parseexec(shell, ptr);
	while (*ptr)
	{
		if (lfsymbol(ptr, "|"))
		{
			if (gettoken(ptr, 0, 0) == 'o')
				cmd = orcmd(cmd, parse_double_node(shell, ptr));
			else
				cmd = pipecmd(cmd, parse_double_node(shell, ptr));
		}
		else if (lfsymbol(ptr, "&"))
		{
			gettoken(ptr, 0, 0);
			cmd = andcmd(cmd, parse_double_node(shell, ptr));
		}
		else
			break ;
	}
	return (cmd);
}
