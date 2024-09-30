/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 13:31:38 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/30 13:39:05 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialize_cmd(t_cmd **ret, t_execcmd **cmd, int *argc)
{
	*ret = execcmd();
	if (!ret)
		return ;
	*cmd = (t_execcmd *)*ret;
	*argc = 0;
}

static t_cmd	*parseblock(t_shell *shell, char **ps, int *check)
{
	t_cmd	*cmd;

	gettoken(ps, 0, 0);
	cmd = parseline(shell, ps);
	gettoken(ps, 0, 0);
	return (cmd);
}

static t_cmd	*parse_subshell(t_shell *shell, char **ps, int *check)
{
	t_sub	*ret;

	ret = malloc(sizeof(*ret));
	if (!ret)
		return (NULL);
	ret->type = SUB;
	ret->cmd = parseblock(shell, ps, check);
	return ((t_cmd *)ret);
}

static int	parse_single_argument(t_execcmd *cmd, char **ps, int argc)
{
	char	*ptr_arg;
	char	*ptr_earg;
	int		token;

	token = gettoken(ps, &ptr_arg, &ptr_earg);
	if (token == 'a')
	{
		cmd->args = args_malloc(argc, ptr_arg, ptr_earg, cmd->args);
		return (1);
	}
	return (0);
}

t_cmd	*parseexec(t_shell *shell, char **ps)
{
	t_cmd		*ret;
	t_execcmd	*cmd;
	int			argc;
	int			check;

	check = 1;
	if (lfsymbol(ps, "("))
	{
		ret = parse_subshell(shell, ps, &check);
		parseredirs_primo(&shell->pipe, &ret, ps, &check);
		return (ret);
	}
	initialize_cmd(&ret, &cmd, &argc); ////////
	parseredirs_primo(&shell->pipe, &ret, ps, &check);
	if (!ret)
		return (NULL);
	while (!lfsymbol(ps, "|)<>&\0"))
	{
		if (!parse_single_argument(cmd, ps, argc))
			break ;
		argc++;
		parseredirs_primo(&shell->pipe, &ret, ps, &check);
	}
	if(cmd->args)
		cmd->args[argc] = 0;
	else
		cmd->args = NULL;

	return (ret);
}
