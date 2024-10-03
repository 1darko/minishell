/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:48:37 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/03 10:48:47 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_and(t_shell *shell, char **ptr)
{
	t_cmd	*cmd;

	cmd = parse_double_node(shell, ptr);
	if (*ptr && lfsymbol(ptr, "&&"))
	{
		gettoken(ptr, 0, 0);
		cmd = andcmd(cmd, parse_and(shell, ptr));
	}
	return (cmd);
}

t_cmd	*parse_or(t_shell *shell, char **ptr)
{
	t_cmd	*cmd;

	cmd = parse_double_node(shell, ptr);
	if (*ptr && lfsymbol(ptr, "||"))
	{
		gettoken(ptr, 0, 0);
		cmd = orcmd(cmd, parse_or(shell, ptr));
	}
	return (cmd);
}

void	nulargs(t_cmd *cmd)
{
	int			i;
	int			j;
	t_execcmd	*exe;

	i = 0;
	exe = (t_execcmd *)cmd;
	while ((exe->args[i]) != 0)
	{
		*exe->args[i] = 0;
		i++;
	}
}

char	*empty_string(void)
{
	char	*str;

	str = malloc(sizeof(char) * 1);
	if (!str)
		return (NULL);
	str[0] = '\0';
	return (str);
}
