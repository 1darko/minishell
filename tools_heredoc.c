/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:13:03 by dakojic           #+#    #+#             */
/*   Updated: 2024/09/27 11:17:44 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_strlen_heredoc(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

static char	*ft_strjoin_heredoc(char const *s1, char const *s2)
{
	size_t	len;
	size_t	cur;
	size_t	i;
	char	*new;

	cur = 0;
	i = 0;
	if (!s2)
		return (NULL);
	len = ft_strlen_heredoc(s1) + ft_strlen_heredoc(s2);
	new = (char *)malloc(sizeof (char) * len + 2);
	if (!new)
		return (NULL);
	while (s1 && s1[cur] != '\0')
	{
		new[cur] = s1[cur];
		cur++;
	}
	while (s2[i] != '\0')
	{
		new[cur++] = s2[i++];
	}
	new[cur] = '\n';
	new[cur + 1] = '\0';
	return (new);
}

static int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s3;
	unsigned char	*s4;
	size_t			cur;

	if (!s1 || !s2)
		return (0);
	cur = 0;
	s3 = (unsigned char *) s1;
	s4 = (unsigned char *) s2;
	if (n == 0)
		return (0);
	while (cur < n && s3[cur] != '\0' && s4[cur] != '\0')
	{
		if (s3[cur] != s4[cur])
			return (s3[cur] - s4[cur]);
		cur++;
	}
	if (cur == n)
		return (0);
	return (s3[cur] - s4[cur]);
}

char	*heredoc_filler(char *end)
{
	char	*buf;
	char	*heredoc;

	heredoc = NULL;
	buf = (char *)readline("heredoc> ");
	if (ft_strncmp(end, buf, ft_strlen(end)) == 0)
	{
		free(buf);
		return (ft_calloc(1, 1));
	}
	while (ft_strncmp(end, buf, ft_strlen(end)) != 0)
	{
		if (buf == NULL)
			return (NULL);
		heredoc = ft_strjoin_heredoc(heredoc, buf);
		free(buf);
		buf = (char *)readline("heredoc> ");
	}
	return (heredoc);
}