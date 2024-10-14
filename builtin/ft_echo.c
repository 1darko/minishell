/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 12:40:04 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/14 14:28:33 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Takes pointer to execcmd struct as arg since its the end product and frees it
// Echo always returns 0 unless write fails


int n_option(char **cmd, int *i, bool *arg)
{
    int j;

    j = 2;
    while(cmd[*i] && cmd[*i][0]== '-' && cmd[*i][1] == 'n')
    {
        while(cmd[*i][j] == 'n')
            j++;
        if(cmd[*i][j] == '\0')
            *arg = true;
        else
            return (0);
        (*i)++;
        j = 2;
    }
    return (0);
}
int ft_echo(t_execs *execs)
{
    int i;
    bool n_arg;

    i = 1;
    n_arg = false;
    n_option(*((t_execcmd *)execs->cmd)->args, &i, &n_arg);
    while((((t_execcmd *)execs->cmd)->args)[i])
    {
        write(1, ((t_execcmd *)execs->cmd)->args[i], ft_strlen(((t_execcmd *)execs->cmd)->args[i]));
        if(((t_execcmd *)execs->cmd)->args[i + 1] != 0)
            write(1, " ", 1);
        i++;
    }
    if(n_arg == false)
        write(1, "\n", 1);
    i = 0;
    while(((t_execcmd *)execs->cmd)->args[i])
        free(((t_execcmd *)execs->cmd)->args[i++]);
    free(((t_execcmd *)execs->cmd)->args);
    ((t_execcmd *)execs->cmd)->args = NULL;
    return (0);
}
// int main()
// {
//     char **cmd = malloc(5 * sizeof(char *));
//     if (!cmd) return 1;

//     cmd[0] = strdup("echo"); // Use strdup to duplicate string literals safely
//     cmd[1] = strdup("-nABCn");
//     cmd[2] = strdup("-n");
//     cmd[3] = strdup("World");
//     cmd[4] = NULL;
//     ft_echo(&cmd);
//     // ft_echo(&cmd);
//     return 0;
// }