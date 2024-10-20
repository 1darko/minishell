#include "../minishell.h"

// unset with no options

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
void ft_unset(t_execs *execs)
{
    int j;

    j = 0;
    if(((t_execcmd *)execs->cmd)->args[1] == NULL)
        return(free_array(((t_execcmd *)execs->cmd)->args));
    free(((t_execcmd *)execs->cmd)->args[0]);
    ft_unset2(&((t_execcmd *)execs->cmd)->args, &((t_shell *)execs->shell)->env, j);
    execfree(execs);
    // free(((t_execcmd *)execs->cmd)->args);
}

// int main()
// {
//     char **env;
//     char **cmd;
//     int i = 0;
//     env = (char **)malloc(sizeof(char *) * 5);
//     env[0] = strdup("OPREM=/RADOVAN/sbin:/sbin");
//     env[1] = strdup("DUMDU=/BOMBOUM           I WANT YOU IN MY ROOM");
//     env[2] = strdup("DUMDUM=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
//     env[3] = strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
//     env[4] = NULL;
//     while(env[i])
//     {
//         printf("Env[%d] : %s\n", i, env[i]);
//         i++;
//     }
//     cmd = (char **)malloc(sizeof(char *) * 4);
//     cmd[0] = strdup("DUMDUM");
//     cmd[1] = strdup("DUMDUM");
//     cmd[2] = strdup("PATH");
//     cmd[3] = NULL;
//     ft_unset(&cmd, &env);
//     i = 0;

//     while(env[i])
//     {
//         printf("Env[%d] : %s\n", i, env[i]);
//         i++;
//     }
//     free_array(env);

//     return 0;
// }