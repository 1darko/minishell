#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*s3;
	unsigned char	*s4;
	size_t			cur;

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


char *minisplit(char *s, int c)
{
    char *new;
    int i;

    i = 0;
    while(s[i] && s[i] != (char)c)
        i++;
    if(s[i] == '\0')
        return (NULL);
    new = (char *)malloc(sizeof(char) * (i + 1));
    i = 0;
    while(s[i] != c)
    {
        new[i] = s[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

char **new_env(char ***env, char *cmd)
{
    char **new;
    int i;
    int j;

    i = 0;
    while((*env)[i] != NULL)
        i++;
    new = (char **)malloc(sizeof(char *) * (i + 2));
    i = 0;
    while((*env)[i] != NULL)
    {
        new[i] = ft_strdup((*env)[i]);
        free((*env)[i]);
        i++;
    }
    new[i] = ft_strdup(cmd);
    new[i + 1] = NULL;
    free(*env);
    return (new);
}


void print_env(char **env)
{
    int i;

    i = 0;
    while(env[i] != NULL)
    {
        printf("declare -x %s\n", env[i]);
        i++;
    }
    return ;
}

int	ft_isalpha(int c)
{
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

void ft_export2(char ***cmd, char ***env)
{
    char *lf;
    int i;
    int j;

    i = 1;
    while(!ft_strncmp((*cmd)[i], "-p", ft_strlen((*cmd)[i])) && (*cmd)[i + 1] != NULL)
       i++;
    while((*cmd)[i] != NULL)
    {
        if(!ft_isalpha((*cmd)[i][0]))
        {
            printf("Minishell: export: `%s': not a valid identifier\n", (*cmd)[i]);
            i++;
            continue;
        }
        lf = minisplit((*cmd)[i], '=');
        if(lf == NULL && i++)
            continue;
        j = 0;
        while((*env)[j] != NULL)
        {
            if(!ft_strncmp(lf, (*env)[j], ft_strlen(lf)))
            {
                free((*env)[j]);
                (*env)[j] = ft_strdup((*cmd)[i]);
                break;
            }
            j++;
        }
        if((*env)[j] == NULL)
            *env = new_env(env, (*cmd)[i]);
        i++;
        free(lf);
    }
}


void ft_export(t_execs *execs)
{
    char *lf;
    int i;
    int j;

    i = 1;
    while(!ft_strncmp(((t_execcmd *)execs->cmd)->args[i], "-p", ft_strlen(((t_execcmd *)execs->cmd)->args[i]) && ((t_execcmd *)execs->cmd)->args[i + 1] != NULL))
       i++;
    if(((t_execcmd *)execs->cmd)->args[1] == NULL || (!ft_strncmp(((t_execcmd *)execs->cmd)->args[i], "-p", ft_strlen(((t_execcmd *)execs->cmd)->args[i]) && ((t_execcmd *)execs->cmd)->args[i + 1] == NULL)))
    {   
        free_array(&((t_execcmd *)execs->cmd)->args[i]);
        print_env(((t_shell *)execs->shell)->env);
        return ;
    }
    ft_export2(&((t_execcmd *)execs->cmd)->args, &((t_shell *)execs->shell)->env);
    i = 0;
    while(((t_execcmd *)execs->cmd)->args[i] != NULL)
    {
        free(((t_execcmd *)execs->cmd)->args[i]);
        i++;
    }
    free(((t_execcmd *)execs->cmd)->args);
    print_env(((t_shell *)execs->shell)->env);
}

// #include  <string.h>
// int main()
// {
//     char **env;
//     char **cmd;
//     char *line;
//     int i;

//     i = 0;
//     env = (char **)malloc(sizeof(char *) * 5);
//     env[0] = ft_strdup("OPREM=/RADOVAN/sbin:/sbin");
//     env[1] = ft_strdup("DUMDU=/BOMBOUM           I WANT YOU IN MY ROOM");
//     env[2] = ft_strdup("DUMDUM=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
//     env[3] = ft_strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
//     env[4] = NULL;
//     cmd = (char **)malloc(sizeof(char *) * 5);
//     cmd[0] = ft_strdup("export");
//     cmd[1] = ft_strdup("-p");
//     cmd[2] = ft_strdup("=");
//     cmd[3] = ft_strdup("BOOM=BOOM");
//     cmd[4] = NULL;
//     ft_export(&cmd, &env);
//     free_array(env);

//     return 0;
// }