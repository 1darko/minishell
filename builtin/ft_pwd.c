/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:15:27 by dakojic           #+#    #+#             */
/*   Updated: 2024/07/22 18:18:51 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(t_execcmd **cmd)
{
    char	*pwd;

    free(*cmd);    
    pwd = getcwd(NULL, 0);
    if(!pwd)
    {
        write(1, "Error: getcwd failed\n", 22);
        return ;
    }
    if(write(4, pwd, strlen(pwd)) < 0 || write(1, "\n", 1) < 0)
        write(1, "Error: write failed\n", 21);
    free(pwd);
}
// int main()
// {
//     t_execcmd *cmd;
//     cmd = malloc(sizeof(t_execcmd));
//     cmd->args[0] = "pwd";
//     cmd->args[1] = NULL;
//     ft_pwd(&cmd);
//     return (0);
// }