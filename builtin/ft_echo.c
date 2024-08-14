/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:40:04 by dakojic           #+#    #+#             */
/*   Updated: 2024/08/13 12:46:03 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Takes pointer to execcmd struct as arg since its the end product and frees it
// Echo always returns 0 unless write fails

// Fix  -nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn, only bunch of Ns
// Fix  -nnnnnEEEEE , bunch of Ns but some random chars also
int n_option(t_execcmd **cmd, int *i, bool *arg)
{
    while(ft_strcmp((*cmd)->args[++(*i)],"-n") == 0)
    {
        if((*cmd)->args[*i + 1] == NULL) // If -n is the only argument
        {
            free(*cmd);
            return 1;
        }
        if(*arg == false)
            *arg = true;
    }
    return (0);
}
int ft_echo(t_execcmd **cmd)
{
    int i;
    bool n_arg;

    i = 0;
    n_arg = false;
    if(n_option(cmd, &i, &n_arg) == 1)
        return (0);
    while((*cmd)->args[i])
    {
        write(1, (*cmd)->args[i], ft_strlen((*cmd)->args[i]));
        if((*cmd)->args[i + 1] != 0)
            write(1, " ", 1);
        i++;
    }
    if(n_arg == false)
        write(1, "\n", 1);
    return (free((*cmd)), 0);
}
int main()
{
    t_execcmd *cmd;
    cmd = malloc(sizeof(t_execcmd));
    char *bla = malloc(sizeof(char) * 5);
    bla = "echo";
    cmd->args[0] = bla;
    cmd->args[1] = "-nm";
    cmd->args[2] = "ho";
    cmd->args[3] = "Hello";
    cmd->args[4] = NULL;
    ft_echo(&cmd);
    ft_echo(&cmd);
    return 0;
}