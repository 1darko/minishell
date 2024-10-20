/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:26:02 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/17 11:19:43 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	findredir(t_cmd *cmd, t_cmd **temp)
{
	*temp = cmd;
	while ((*temp)->type == REDIR || (*temp)->type == HERE)
	{
		if (((t_redircmd *)(*temp))->cmd->type != REDIR
		&& ((t_redircmd *)(*temp))->cmd->type != HERE)
			break ;
		*temp = ((t_redircmd *)(*temp))->cmd;
	}
}

void	parseredirs_primo(t_herepipe **pipes, t_cmd **cmd, char **ps, int *c)
{
	char	*ptr_file;
	char	*ptr_endfile;
	int		token;
	t_cmd	*temp;

	findredir(*cmd, &temp);
	if (*c && lfsymbol(ps, "<>"))
	{
		*c = 0;
		token = gettoken(ps, &ptr_file, &ptr_endfile);
		gettoken(ps, &ptr_file, &ptr_endfile);
		if (token == '>')
			*cmd = redircmd_in(*cmd, ptr_file, ptr_endfile, 1);
		else if (token == '<')
			*cmd = redircmd_out(*cmd, ptr_file, ptr_endfile, 0);
		else if (token == '+')
			*cmd = redircmd_append(*cmd, ptr_file, ptr_endfile, 1);
		else if (token == 'h')
			*cmd = redircmd_here(pipes, *cmd);
		if (!cmd)
			return ;
		parseredirs_er(pipes, *cmd, ps);
	}
	else
		parseredirs_er(pipes, temp, ps);
}

t_cmd	*parseredirs_er(t_herepipe **pipes, t_cmd *cmd, char **ps)
{
	char	*ptr_file;
	char	*ptr_endfile;
	int		token;
	t_cmd	*temp;

	temp = cmd;
	while (lfsymbol(ps, "><"))
	{
		token = gettoken(ps, &ptr_file, &ptr_endfile);
		gettoken(ps, &ptr_file, &ptr_endfile);
		if (token == '>')
			temp = redircmd_in2(temp, ptr_file, ptr_endfile, 1);
		else if (token == '<')
			temp = redircmd_out2(temp, ptr_file, ptr_endfile, 0);
		else if (token == '+')
			temp = redircmd_append2(temp, ptr_file, ptr_endfile, 1);
		else if (token == 'h')
			temp = redircmd_here2(pipes, temp);
		if (!temp)
			return (NULL);
	}
	return (cmd);
}
