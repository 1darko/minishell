/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:17:13 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/16 12:15:27 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static long long ft_atol(char *str)
{
    long long ret;
    int i;
    int neg;

	i = 0;
	neg = 1;
	ret = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		ret = ret * 10 + (str[i] - 48);
		i++;
	}
	return (ret * neg);
}

static long long ret_exit(char *x)
{
    int i;

    i = 0;
    while(x[i] != '\0')
    {
        if(!(x[i] >= '0' && x[i] <= '9'))
        {
            write(2, "Error: Invalid exit argument. Numeric argument required.\n", 58);
            return (2);
        }
        i++;
    }
    return (ft_atol(x));
}

void ft_exit(t_execs *ex)
{
    unsigned int ret;

    ret = 0;
    if(((t_execcmd *)ex->cmd)->args[1])
        ret = (unsigned int)ret_exit(((t_execcmd *)ex->cmd)->args[1]);
    // ft_listfree(ex->fds, fdsfree);
    execfree(ex);
    write(2,"exit\n",5);
    exit(ret);
}