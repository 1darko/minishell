/*
    cd arg1 arg2    bash: cd: too many arguments
    cd ~ == cd -- == cd $HOME
    cd - == cd $OLDPWD
    cd .. == cd ../
    cd . == cd ./
    cd / == cd // == cd ////////////////////////////
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	free_array(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return ;
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t				i;
	unsigned char		*new;

	i = nmemb * size;
	if (i == 0)
	{
		new = malloc(0);
		if (!new)
			return (0);
		*new = 0;
		return (new);
	}
	if (i < nmemb || i < size)
		return (0);
	new = malloc(i);
	if (!new)
		return (0);
	while (i--)
		new[i] = 0;
	return (new);
}
int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*s3;
	unsigned char	*s4;

	s3 = (unsigned char *)s1;
	s4 = (unsigned char *)s2;
	while (*s3 == *s4 && *s3 != '\0')
	{
		++s3;
		++s4;
	}
	return (*s3 - *s4);
}

int	ft_strcmp2(const char *s1, const char *s2)
{
	unsigned char	*s3;
	unsigned char	*s4;

	s3 = (unsigned char *)s1;
	s4 = (unsigned char *)s2;
	while (*s3 == *s4 && *s3 != '\0')
	{
		++s3;
		++s4;
	}
	if(*s3 == '=' && *s4 == '\0')
        return (0);
    return (1);
}

static char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	j;
	char	*copy;

	if (start > ft_strlen(s))
		return (ft_calloc(1, 1));
	if (len + start <= ft_strlen(s))
		copy = (char *)malloc(sizeof(char) * (len + 1));
	else
		copy = (char *)malloc(sizeof(char) * (ft_strlen(s + start) + 1));
	if (!copy)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len)
		{
			copy[j] = s[i];
			j++;
		}
		i++;
	}
	copy[j] = 0;
	return (copy);
}

char	*get_env(char *name, char **env)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		new = ft_substr(env[i], 0, j);
		if (ft_strcmp(new, name) == 0)
		{
			free(new);
			return (env[i] + j + 1);
		}
		free(new);
		i++;
	}
	return (NULL);
}
int size_pwd(char **env)
{
    int i;

    i = 0;
    while(env[i])
    {
        if(!ft_strcmp2(env[i], "PWD"))
            break;
        i++;
    }
    return (ft_strlen(env[i]));
}
void    ft_cd(char **cmd, char **env) //Update PWD et OLDPWD a chaque move
{
    char *oldpwd;
    char *newpwd;

    oldpwd = malloc(sizeof(char) * (size_pwd(env) + 1));
    getcwd(oldpwd, size_pwd(env) + 1); // Goes on OLDPWD

    if(cmd[0] && (cmd[1] == NULL || !ft_strcmp(cmd[1], "~") || !ft_strcmp(cmd[1], "--")))
        chdir(get_env("HOME", env));
    if(cmd[0] && !ft_strcmp(cmd[1], "-"))
        chdir(get_env("OLDPWD", env));
    
    newpwd = malloc(sizeof(char) * (size_pwd(env) + 1));
    getcwd(newpwd, size_pwd(env) + 1); // Ca devient PWD

    free_array(cmd);
    return ;
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
  
    char s[100]; 

    char **str = malloc(sizeof(char *) *3);
    str[0] = strdup("cd");
    str[1] = strdup("-");
    str[2] = NULL;

    ft_cd(str, env);

    return (0);
}