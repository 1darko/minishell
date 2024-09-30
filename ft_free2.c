/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 13:11:07 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 13:11:18 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(t_execcmd *ex)
{
	int	i;

	i = 0;
	while (ex->args[i] != 0)
	{
		free(ex->args[i]);
		i++;
	}
	free(ex->args[i]);
}

void	free_exec(t_execcmd *ex)
{
	free_args(ex);
	free(ex);
}

void	free_sub(t_sub *sub)
{
	tree_free(&sub->cmd);
	free(sub);
}

void	free_redir(t_redircmd *re)
{
	tree_free(&re->cmd);
	free(re);
}

void	free_double(t_doublecmd *db)
{
	tree_free(&db->left);
	tree_free(&db->right);
	free(db);
}
