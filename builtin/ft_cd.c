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

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len;
	size_t	cur;
	size_t	i;
	char	*new;

	cur = 0;
	i = 0;
	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	new = (char *)malloc(sizeof (char) * len + 1);
	if (!new)
		return (NULL);
	while (s1[cur] != '\0')
	{
		new[cur] = s1[cur];
		cur++;
	}
	while (s2[i] != '\0')
	{
		new[cur++] = s2[i++];
	}
	new[cur] = '\0';
	return (new);
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

char	*ft_substr(char const *s, unsigned int start, size_t len)
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
static int size_pwd(char **env)
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

static void	ft_getcwd(char **pwd, int size)
{
	*pwd = malloc(sizeof(char) * (size + 1));
	getcwd(*pwd, size + 1);
	return ;
}

static int ft_move(char **cmd, char **env)
{
	if(cmd[0] && (cmd[1] == NULL || !ft_strcmp(cmd[1], "~") || !ft_strcmp(cmd[1], "--")))
        chdir(get_env("HOME", env));
    else if(cmd[0] && !ft_strcmp(cmd[1], "-"))
	{	
        chdir(get_env("OLDPWD", env));
		printf("%s\n", get_env("OLDPWD", env));
	}
	else if(cmd[0] && cmd[1])
		if(chdir(cmd[1]) != 0)
		{
			printf("cd: %s: No such file or directory\n", cmd[1]);
			free_array(cmd);
			return (1);
		}
	free_array(cmd);
	return (0);
}
char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	cur;
	char	*dup;

	cur = 0;
	len = ft_strlen((char *)s);
	dup = (char *)malloc(sizeof(char) * len + 1);
	if (dup == 0)
		return (NULL);
	while (s[cur] != '\0')
	{
		dup[cur] = s[cur];
		cur++;
	}
	dup[cur] = '\0';
	return (dup);
}

static void ft_switch_pwd(char *newpwd, char *oldpwd, char ***env)
{
	int i;
	int j;
	char *tmp;

	i = -1;
	while((*env)[++i])
	{
		j = 0;
		while((*env)[i][j] && (*env)[i][j] != '=')
			j++;
		tmp = ft_substr((*env)[i], 0, j);
		if(!ft_strcmp2(tmp, "PWD"))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup("PWD=");
			(*env)[i] = ft_strjoin((*env)[i], newpwd);
		}
		if(!ft_strcmp2(tmp, "OLDPWD"))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup("OLDPWD=");
			(*env)[i] = ft_strjoin((*env)[i], oldpwd);
		}
		free(tmp);
	}
}

void    ft_cd(char **cmd, char **env)
{
    char *oldpwd;
    char *newpwd;
	int check;

	if(cmd[2])
	{
		printf("Minishell: cd: too many arguments\n"); /// Changer le errno a 1???
		free_array(cmd);
		return ;
	}
	ft_getcwd(&oldpwd, size_pwd(env));
	if(ft_move(cmd, env) == 1)
	{
		free(oldpwd);
		return ;
	}
	ft_getcwd(&newpwd, size_pwd(env));
    ft_switch_pwd(newpwd, oldpwd, &env);
	free(oldpwd);
	free(newpwd);
    return ;
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
  
    char s[100]; 

    char **str = malloc(sizeof(char *) *3);
    str[0] = strdup("cd");
    str[1] = strdup("/home/dakojic/code");
    str[2] = NULL;

    ft_cd(str, env);

    return (0);
}