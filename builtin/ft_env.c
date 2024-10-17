#include "../minishell.h"

// env with no options or arguments

void ft_env(t_execs *execs)
{
    int i;

    i = 0;
    if(!execs->shell->env[0])
    {
        execs->ret = 1;
        execfree(execs);
    }
    while(execs->shell->env[i])
    {
        printf("%s\n", ((t_shell *)execs->shell)->env[i]);
        i++;
    }
    execfree(execs);
}

// int main()
// {
//     char **env;
//     char **cmd;
//     env = (char **)malloc(sizeof(char *) * 2);
//     env[0] = strdup("PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
//     env[1] = NULL;
//     cmd = (char **)malloc(sizeof(char *) * 2);
//     cmd[0] = strdup("env");
//     cmd[1] = NULL;
//     env_display(cmd, env);
//     return 0;
// }