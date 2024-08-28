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
#include <unistd.h>

// void	free_array(char **s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 	{
// 		free(s[i]);
// 		i++;
// 	}
// 	free(s);
// 	return ;
// }
void	ft_pwd(char **cmd)
{
    char	*pwd;
    free_array(cmd);
    pwd = getcwd(NULL, 0);
    if(!pwd)
    {
        write(1, "Minishell: error: getcwd failed\n", 22);
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