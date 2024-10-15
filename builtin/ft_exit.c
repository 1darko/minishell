/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:17:13 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/15 13:56:40 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_exit(t_execs *ex)
{
    unsigned int ret;

    ret = 0;
    if(((t_execcmd *)ex->cmd)->args[1])
        ret = (unsigned int)((t_execcmd *)ex->cmd)->args[1];
    ft_listfree(ex->fds, fdsfree);
    execfree(ex);
    write(2,"exit\n",5);
    exit(ret);
}