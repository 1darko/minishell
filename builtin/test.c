#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void print_env(char **env)
{
    int i;

    i = -1;
    while(env[++i])
        printf("declare -x %s\n", env[i]);
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

void add_env(char ***env, const char *new_var) {
    int size = 0;
    while ((*env)[size]) {
        size++;
    }
    
    char **new_env = realloc(*env, (size + 2) * sizeof(char *));
    if (!new_env) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    new_env[size] = ft_strdup(new_var);
    new_env[size + 1] = NULL;

    *env = new_env;
}

void export_unset(char ***cmd, char ***env) {
    if ((*cmd)[0] == NULL)
        return;

    printf("cmd[0]: %s\n", (*cmd)[0]);

    int i = 0;
    while ((*env)[i]) {
        int j = 0;
        while ((*env)[i][j] == (*cmd)[0][j] && (*env)[i][j] != '=' && (*cmd)[0][j] != '\0') {
            j++;
        }
        
        if ((*env)[i][j] == '=' && (*cmd)[0][j] == '=') {
            free((*env)[i]);
            printf("cmd[0]: %s\n", (*cmd)[0]);
            (*env)[i] = ft_strdup((*cmd)[0]);
            return;
        }
        i++;
    }

    add_env(env, (*cmd)[0]);

    export_unset(cmd + 1, env);
}

void ft_export(char ***cmd, char ***env) {
    
    int i = 0;
    while((*cmd)[i]) {
        printf("cmd[%d]: %s\n", i, (*cmd)[i]);
        i++;
    }

    if (ft_strcmp((*cmd)[1], "-n") == 0 && (*cmd)[2] == NULL) {
        print_env(*env);
    } else if (ft_strcmp((*cmd)[1], "-n") == 0 && (*cmd)[2] != NULL) {
        printf("cmd +1: %s\n", (*cmd)[2]);
        export_unset(cmd + 2, env);
    }
}

int main() {
    char **env = (char **)malloc(sizeof(char *) * 5);
    char **cmd = (char **)malloc(sizeof(char *) * 5);

    env[0] = ft_strdup("x=1");
    env[1] = ft_strdup("c=2");
    env[2] = ft_strdup("e=3");
    env[3] = ft_strdup("g=4");
    env[4] = NULL;

    cmd[0] = ft_strdup("export");
    cmd[1] = ft_strdup("-n");
    cmd[2] = ft_strdup("x=50");
    cmd[3] = NULL;

    ft_export(&cmd, &env);

    int i = 0;
    while (env[i]) {
        printf("BLAH %s\n", env[i++]);
    }

    // Free allocated memory
    for (i = 0; env[i]; i++) {
        free(env[i]);
    }
    free(env);

    for (i = 0; cmd[i]; i++) {
        free(cmd[i]);
    }
    free(cmd);

    return 0;
}
