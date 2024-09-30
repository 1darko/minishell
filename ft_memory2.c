/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memory2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 16:09:08 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 16:09:30 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_strdup_arg(char *start, char *end)
{
	char	*copy;
	int		i;
	int		size;

	i = 0;
	size = 0;
	while ((start + size) != end && start[size] != '\0')
		size++;
	copy = malloc(sizeof(char) * (size + 1));
	if (!copy)
		return (NULL);
	while (i != size)
	{
		copy[i] = start[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	**args_malloc(int argc, char *ptr_arg, char *ptr_earg, char **args)
{
	char	**ret;
	int		i;

	i = 0;
	ret = malloc(sizeof(char *) * (argc + 2));
	if (argc > 0)
	{
		while (args[i])
		{
			ret[i] = ft_strdup(args[i]);
			i++;
		}
	}
	ret[i] = ft_strdup_arg(ptr_arg, ptr_earg);
	ret[++i] = NULL;
	return (ret);
}

char	*ft_filecpy(char *file, char *efile)
{
	char	*copy;
	int		i;
	int		j;

	i = 0;
	if (!file || !efile)
		return (NULL);
	while (file[i] && file[i] != *efile)
		i++;
	copy = malloc(sizeof(char) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (file[i] && file[i] != *efile)
	{
		copy[i] = file[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
