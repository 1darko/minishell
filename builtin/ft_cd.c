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
#include "../minishell.h"

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

void    ft_cd(t_execs *execs)
{
    char *oldpwd;
    char *newpwd;
	int check;

	if(((t_execcmd *)execs->cmd)->args[2])
	{
		printf("Minishell: cd: too many arguments\n"); /// Changer le errno a 1???
		free_array(((t_execcmd *)execs->cmd)->args);
		return ;
	}
	ft_getcwd(&oldpwd, size_pwd(execs->shell->env));
	if(ft_move(((t_execcmd *)execs->cmd)->args, execs->shell->env) == 1)
	{
		free(oldpwd);
		return ;
	}
	ft_getcwd(&newpwd, size_pwd(execs->shell->env));
    ft_switch_pwd(newpwd, oldpwd, &execs->shell->env);
	free(oldpwd);
	free(newpwd);
    return ;
}

// int main(int ac, char **av, char **env)
// {
//     (void)ac;
//     (void)av;
  
//     char s[100]; 

//     char **str = malloc(sizeof(char *) *3);
//     str[0] = strdup("cd");
//     str[1] = strdup("/home/dakojic/code");
//     str[2] = NULL;

//     ft_cd(str, env);

//     return (0);
// }