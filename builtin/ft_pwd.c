/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:15:27 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/17 12:59:12 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>

void	ft_pwd(t_execs *execs)
{
    char	*pwd;
    // free_array(((t_execcmd *)execs->cmd)->args);
    // ((t_execcmd *)execs->cmd)->args =NULL;
    execfree(execs);
    pwd = getcwd(NULL, 0);
    if(!pwd)
    {
        write(1, "Minishell: error: getcwd failed\n", 22);
        execs->ret = 1;
        return ;
    }
    if(write(1, pwd, strlen(pwd)) < 0 || write(1, "\n", 1) < 0)
        write(1, "Error: write failed\n", 21);
    free(pwd);
}
// int main()
// {
//     char **cmd;
//     cmd = (char **)malloc(sizeof(char *) * 3);
//     cmd[0] = strdup("DUMDUM");
//     cmd[1] = strdup("PATH");
//     cmd[2] = NULL;
//     ft_pwd(cmd);
//     return (0);
// }