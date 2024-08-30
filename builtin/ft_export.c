#include "../minishell.h"

// export with no options

// char **bla;
// bla[0] = "export";
// bla[1] = "a=b";
// bla[2] = NULL;

// export with -p option
// char **bla;
// bla[0] = "export";
// bla[1] = "-p";
// bla[2] = a;
// bla[3] = NULL;

// export with -n option
// ft_unset(bla, &env);

// NEED TO FIX THE ADD ENV BLA BLA BLA, NOT WORKING RN IN ANYWAY SHAPE OR FORM
// NOTHING

int ft_strlen(const char *s)
{
    int i;

    i = 0;
    while(s[i])
        i++;
    return (i);
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

void	free_array(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
        printf("On essaie de free %s\n", s[i]);
		free(s[i]);
		i++;
	}
	free(s);
	return ;
}


static char **unset_env(char ***str, int skip)
{
    char **copy;
    int i;
    int j;

    i = 0;
    j = 0;
    while ((*str)[i])
        i++;
    copy = malloc(sizeof(char *) * i);
    if (!copy)
        return (NULL);
    i = 0;
    while ((*str)[i])
    {
        if (skip == i)
        {
            i++;
            continue;
        }
        copy[j++] = ft_strdup((*str)[i++]);
    }
    copy[j] = NULL;
    return (copy);
}


static int envcheck(char *path, char *env, int size)
{
    int i;

    i = 0;
    while(env[i] && path[i] && env[i] == path[i])
        i++;
    if(size != i || env[i] != '=' || path[i] != '\0')
        return(1);
    return (0);
}
void ft_unset2(char ***cmd, char ***env, int j)
{   
    int i;
    int size;
    char **cpy;

    while((*cmd)[++j])
    {
        i = -1;
        size = ft_strlen((*cmd)[j]);
        while((*env)[++i])  
            if(!envcheck((*cmd)[j], (*env)[i], size))
            {
                size = -1;
                break;
            }
        if((*env)[i] == NULL && (*cmd)[j + 1] == NULL)
            return (free_array(*cmd));
        if(size == -1)
        {
            cpy = unset_env(env, i);
            free_array(*env);
            *env = cpy;
            free((*cmd)[j]);
        }
    }
}
void ft_unset(char ***cmd, char ***env)
{
    int j;

    j = 0;
    if((*cmd)[1] == NULL)
        return(free_array(*cmd));
    free((*cmd)[0]);
    ft_unset2(cmd, env, j);
    free((*cmd));
}

////////////////////////////////////////////

// char	*ft_strdup(const char *s)
// {
// 	size_t	len;
// 	size_t	cur;
// 	char	*dup;

// 	cur = 0;
// 	len = ft_strlen((char *)s);
// 	dup = (char *)malloc(sizeof(char) * len + 1);
// 	if (dup == 0)
// 		return (NULL);
// 	while (s[cur] != '\0')
// 	{
// 		dup[cur] = s[cur];
// 		cur++;
// 	}
// 	dup[cur] = '\0';
// 	return (dup);
// }

static int envcheck_export(char *path, char *env, int size)
{
    int i;

    i = 0;
    while(env[i] && path[i] && env[i] == path[i] && i < size)
        i++;
    if(env[i] == '=' && path[i] == '=')
        return(1);
    return (0);
}
void print_env(char **env)
{
    int i;

    i = -1;
    while(env[++i])
        printf("declare -x %s\n", env[i]);
}

void add_env(char ***env, char *cmd)
{
    int i;
    char **cpy;

    i = 0;
    while((*env)[i])
        i++;
    cpy = malloc(sizeof(char *) * (i + 2));
    i = -1;
    while((*env)[++i])
        cpy[i] = ft_strdup((*env)[i]);
    cpy[i] = ft_strdup(cmd);
    cpy[i + 1] = NULL;
    free_array(*env);
    *env = cpy;
}

void replace_env(char ***env, char ***cmd, int i, int j)
{   
    int y;
    int size;
    char **cpy;
    
    y = 0;
    size = 0;
    while((*env)[size])
        size++;
    cpy = malloc(sizeof(char *) * (size + 1));
    y = -1;
    while((*env)[++y])
    {
        if(y == i)
        {
            cpy[y] = ft_strdup((*cmd)[j]);
            continue;
        }
        cpy[y] = ft_strdup((*env)[y]);
    }
    cpy[y] = NULL;
    // free_array(*env);
    *env = cpy;
}

void ft_export(char ***cmd, char ***env)
{
    int i;
    int j;
    int size;
    char **cpy;

    if(ft_strcmp((*cmd)[1], "-p") == 0)
        print_env(*env);
    else if(ft_strcmp((*cmd)[1], "-n") == 0){printf("inside this\n");
        return((free((*cmd)[0] )));ft_unset(cmd + 1, env);}
    else if((*cmd)[1] == NULL)
        return(free_array(*cmd));
    j = 0;
    while((*cmd)[++j])
    {
        i = -1;
        size = 0;
        while((*cmd)[j][size] != '=')
            size++;
        while((*env)[++i])
        {
            if(envcheck_export((*cmd)[j], (*env)[i], size))
                break;
        }
        if((*env)[i] == NULL)
            add_env(env, (*cmd)[j]);
        else
            replace_env(env, cmd, i, j);
    }
}

int main()
{
    char **env;
    char **cmd;
    int i = 0;
    env = (char **)malloc(sizeof(char *) * 5);
    cmd = (char **)malloc(sizeof(char *) * 5);
    env[0] = "a=1";
    env[1] = "c=2";
    env[2] = "e=3";
    env[3] = "g=4";
    env[4] = NULL;
    while(env[i])
        printf("%s\n", env[i++]);
    printf("\n");
    printf("\n");
    cmd[0] = "export";
    cmd[1] = "-n";
    cmd[2] = "a";
    cmd[3] = NULL;
    ft_export(&cmd, &env);
    while(env[i])
        printf("BLAH %s\n", env[i++]);
    return 0;
}