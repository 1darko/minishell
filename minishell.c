
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
// #include "minishell.h"


#define MAXARGS 50
#define MAXLINE 1500001

typedef enum e_lex
{
    LEX_START = 0,
    LEX_WORD = 1,
    LEX_REDIROUT = 2,
    LEX_REDIRIN = 3,
    LEX_AND = 4,   
    LEX_PIPE = 5,
    LEX_OPEN = 6,
    LEX_CLOSE = 7,
    LEX_OR = 8,
    LEX_HERE = 9,
    LEX_APPEND = 10
}   t_lex;
typedef enum e_type
{
    SHELL,
    EXEC,
    REDIR,
    PIPE,
    SUB,
    AND,
    OR,
    HERE,
    EMPTY
}   t_type;
// Structs 
typedef struct s_cmd
{
    int type;
}   t_cmd;

typedef struct s_env
{
    char **env;
}   t_env;

typedef struct s_sub
{
    int type;
    t_cmd *cmd;
}   t_sub;

typedef struct s_var
{
    char *alias;
    char *value;
    struct s_var *next;
}   t_var;

typedef struct s_herepipe
{
    char *str;
    int stored;
    struct s_herepipe *next;
} t_herepipe;

typedef struct s_shell
{
    int type;
    t_cmd *tree;
    t_var *var;
    t_env *env;
    t_herepipe *pipe;
}   t_shell;


typedef struct s_execcmd
{
    int type;
    char *args[MAXARGS];
    char *eargs[MAXARGS];
}   t_execcmd;

typedef struct s_pipecmd
{
    int type;
    t_cmd *left;
    t_cmd *right;
}   t_pipecmd;

typedef struct s_redircmd
{
    int type;
    t_cmd *cmd;
    char *file;
    char *efile;
    int mode;
    int fd;
    char *heredoc;
}   t_redircmd;

typedef struct s_andcmd
{
    int type;
    t_cmd *left;
    t_cmd *right;
}   t_andcmd;

typedef struct s_orcmd
{
    int type;
    t_cmd *left;
    t_cmd *right;
}   t_orcmd;

typedef struct s_doublecmd
{
    int type;
    t_cmd *left;
    t_cmd *right;
}   t_doublecmd;

// ??????????????? A VOIR ?????
typedef struct s_herecmd
{
    int type;
    t_cmd *cmd;
    int id;
}   t_herecmd;


typedef struct s_lexer
{
    int type;
    char *heredoc;
    struct s_lexer *next;
    struct s_lexer *prev;
}   t_lexer;


int type_check(t_lexer *lex, int *i, t_shell **shell);
t_cmd *redircmd_here2(t_herepipe **pipe, t_cmd *cmd);
t_cmd *redircmd_here(t_herepipe **pipe, t_cmd *cmd);
void findredir(t_cmd *cmd, t_cmd **temp);
int init_heredoc(t_lexer *lex, t_shell **shell);
t_cmd *parse_or(t_shell *shell, char **ptr);
t_cmd *parse_and(t_shell *shell, char **ptr);
t_cmd *parseline(t_shell *shell, char **ptr);
void nulterminator(t_cmd *cmd);
void nulargs(t_cmd *cmd);
void nulredir(t_cmd *cmd);
void ft_bzero(void *s, size_t n);
int ft_strlen(const char *s);
char *ft_strchr(char *s, char c);
int lfsymbol(char **ptr, char *c);
t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *execcmd(void);
t_cmd *pipecmd(t_cmd *left, t_cmd *right);
t_cmd *parseredirs_er(t_herepipe **pipes, t_cmd *cmd, char **ps);
void    parseredirs_primo(t_herepipe **pipes, t_cmd **cmd, char **ps, int *check);
int gettoken(char **ptr, char **ptr_token, char **ptr_endtoken);
void problem(char *s);
t_cmd *parseblock(t_shell *shell, char **ps,  int *check);
t_cmd *parseexec(t_shell *shell, char **ps);
t_cmd *parse_double_node(t_shell *shell, char **ptr);
void *parsecmd(t_shell **shell, char *str);
t_cmd *parse_all_redirections(t_cmd *cmd, char **ps);
int parse_single_argument(t_execcmd *cmd, char **ps, int argc);
void initialize_cmd(t_cmd **ret, t_execcmd **cmd, int *argc);
int lexing_check(t_shell **shell ,t_lexer *lexer);
int lexer(t_shell **shell, char *str);
void tree_free(t_cmd **tree);
// Pipex 
void ft_exec(t_cmd *tree, char **env);
char	*get_path(char *cmd, char **env);
static char	*ft_strjoin(char const *s1, char const *s2);
char	*get_env(char *name, char **env);
static char	*ft_substr(char const *s, unsigned int start, size_t len);
void	free_array(char **s);
int sub_lexer(t_lexer **lex, char *str, int *i);
void free_lexer(t_lexer *lexer);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

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


// Libft needed functions

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


char *ft_strchr(char *s, char c)
{
    while(*s)
    {
        if(*s == c)
            return (s);
        s++;
    }
    return (0);
}

void ft_bzero(void *s, size_t n)
{
    unsigned char *p;

    p = s;
    while(n--)
        *p++ = 0;
}

int ft_strlen(const char *s)
{
    int i;

    i = 0;
    while(s[i])
        i++;
    return (i);
}

static int	wdcount(const char *str, char c, int *tab, int check)
{
	int	cur;
	int	words;

	if (check == 1)
	{
		tab[0] = 0;
		tab[1] = 0;
	}
	cur = 0;
	words = 0;
	while (str[cur])
	{
		if (str[cur] != c && (str[cur + 1] == c || str[cur + 1] == '\0'))
			words++;
		cur++;
	}
	return (words);
}

static char	*newstr(int start, char c, const char *str, int count)
{
	char	*copy;
	int		i;

	i = 0;
	if (!str)
		return (0);
	copy = (char *)malloc(sizeof(char) * count + 1);
	if (!copy)
		return (0);
	while (str[start] != '\0' && str[start] != c)
	{
		copy[i] = str[start];
		i++;
		start++;
	}
	copy[i] = '\0';
	return (copy);
}

static int	checker(const char *str, int *i, char c)
{
	if ((str[*i] == c && str[*i - 1] != c) && *i != 0)
		return (1);
	if ((str[*i] == 0 && str[*i - 1] != c) && *i != 0)
		return (1);
	return (0);
}

static int	real_split(char **copy, const char *str, int *i, char c)
{
	int	count[2];

	wdcount(str, c, count, 1);
	while (str[++(*i)])
	{
		if (checker(str, i, c))
		{
			copy[count[0]] = newstr(*i - count[1], c, str, count[1]);
			if (!copy[count[0]])
				return (1);
			(count[0])++;
			count[1] = 0;
		}
		if (str[*i] != c && str[*i] != 0)
			count[1]++;
	}
	if (checker(str, i, c))
	{
		copy[count[0]] = newstr(*i - count[1], c, str, count[1]);
		if (!copy[count[0]])
			return (1);
		(count[0])++;
	}
	copy[count[0]] = NULL;
	return (0);
}

char	**ft_split(char const *str, char c)
{
	int		i;
	char	**copy;
	int		j;

	j = 0;
	i = -1;
	if (!str)
		return (NULL);
	copy = malloc(sizeof(char *) * (wdcount(str, c, (int *)0, 0) + 1));
	if (!copy)
		return (NULL);
	if (real_split(&(*copy), str, &i, c))
	{
		while (copy[j])
			free(copy[j++]);
		free(copy);
		return (0);
	}
	return (copy);
}


int lfsymbol(char **ptr, char *c)
{
    char *temp;

    temp = *ptr;
    while(*temp && ft_strchr(" \t\n\r\v", *temp))
        temp++;
    *ptr = temp;
    return (*ptr && ft_strchr(c, *temp));
}


// Constructors

char *ft_filecpy(char *file, char *efile)
{
    char *copy;
    int i;
    int j;
    i = 0;

    if(!file || !efile)
        return (NULL);
    while(file[i] && file[i] != *efile)
        i++;
    copy = malloc(sizeof(char) * (i + 1));
    if(!copy)
        problem("malloc failed");
    i = 0;
    while(file[i] && file[i] != *efile)
    {
        copy[i] = file[i];
        i++;
    }
    copy[i] = '\0';
    return (copy);
}

t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_RDONLY;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_WRONLY | O_CREAT | O_TRUNC;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;
    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_WRONLY | O_CREAT | O_APPEND;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_in2(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;
    t_redircmd *temp;    


    temp = (t_redircmd *)((t_redircmd *)cmd)->cmd;
    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_WRONLY | O_CREAT | O_TRUNC;
    redir->fd = fd;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return((t_cmd *)redir);
}





t_cmd *redircmd_out2(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_RDONLY;
    redir->fd = fd;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return((t_cmd *)redir);
}


t_cmd *redircmd_append2(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;
    t_redircmd *temp;

    temp = (t_redircmd *)((t_redircmd *)cmd)->cmd;
    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = REDIR;
    redir->file = file;
    redir->file = ft_filecpy(file, efile);
    redir->mode = O_WRONLY | O_CREAT | O_APPEND;
    redir->fd = fd;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return((t_cmd *)redir);
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

t_cmd *redircmd_here(t_herepipe **pipe, t_cmd *cmd)
{
    t_redircmd *redir;
    t_herepipe *temp;

    temp = (*pipe)->next;
    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = HERE;
    redir->cmd = cmd;
    redir->mode = O_RDONLY;
    (*pipe)->stored = 1;
    if((*pipe)->str)
        redir->heredoc = ft_strdup((*pipe)->str);
    free((*pipe)->str);
    free(*pipe);
    *pipe = temp;
    return ((t_cmd *)redir);
}

t_cmd *redircmd_here2(t_herepipe **pipe, t_cmd *cmd)
{
    t_redircmd *redir;
    t_herepipe *temp;

    temp = (*pipe)->next;
    redir = malloc(sizeof(*redir));
    if(!redir)
        return (NULL);
    ft_bzero(redir, sizeof(*redir));
    redir->type = HERE;
    redir->cmd = cmd;
    redir->file = NULL;
    redir->efile = NULL;
    redir->mode = O_RDONLY;
    redir->fd = 0;
    (*pipe)->stored = 1;
    if((*pipe)->str)
        redir->heredoc = ft_strdup((*pipe)->str);
    free((*pipe)->str);
    free(*pipe);
    *pipe = temp;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return ((t_cmd *)redir);
}

t_cmd *execcmd(void)
{
    t_execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_bzero(cmd, sizeof(*cmd));
    cmd->type = EXEC;
    return ((t_cmd *)cmd);
};

t_cmd *pipecmd(t_cmd *left, t_cmd *right)
{
    t_pipecmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_bzero(cmd, sizeof(*cmd));
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}


t_cmd *andcmd(t_cmd *left, t_cmd *right)
{
    t_andcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_bzero(cmd, sizeof(*cmd));
    cmd->type = AND;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}

t_cmd *orcmd(t_cmd *left, t_cmd *right)
{
    t_orcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_bzero(cmd, sizeof(*cmd));
    cmd->type = OR;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}

t_cmd *parse_subshell(t_shell *shell, char **ps, int *check) 
{
    t_sub *ret;
    ret = malloc(sizeof(*ret));
    ret->type = SUB;
    ret->cmd = parseblock(shell, ps, check);
    return ((t_cmd *)ret);
}
// Parsing functions

void    parseredirs_primo(t_herepipe **pipes, t_cmd **cmd, char **ps, int *check)
{
    char *ptr_file;
    char *ptr_endfile;
    int token;
    t_cmd *temp;

    findredir(*cmd, &temp);
    if (*check && lfsymbol(ps, "<>"))
    {
        *check = 0;
        token = gettoken(ps, &ptr_file, &ptr_endfile);
        if(gettoken(ps,  &ptr_file, &ptr_endfile) != 'a')
            problem("missing file for redirection");
        if(token == '>')
            *cmd = redircmd_in(*cmd, ptr_file, ptr_endfile, 1);
        else if(token == '<')
            *cmd = redircmd_out(*cmd, ptr_file, ptr_endfile, 0);
        else if(token == '+')
            *cmd = redircmd_append(*cmd, ptr_file, ptr_endfile, 1);
        else if(token == 'h')
        {
            *cmd = redircmd_here(pipes, *cmd);
        }
        else
            problem("this aint a redirection");
        parseredirs_er(pipes, *cmd, ps);
    }
    else
        parseredirs_er(pipes, temp, ps);
}

t_cmd *parseredirs_er(t_herepipe **pipes, t_cmd *cmd, char **ps)
{
    char *ptr_file;
    char *ptr_endfile;
    int token;
    t_cmd *temp;

    temp = cmd;
    while(lfsymbol(ps, "><"))
    {
        token = gettoken(ps, &ptr_file, &ptr_endfile);
        if(gettoken(ps, &ptr_file, &ptr_endfile) != 'a')
            problem("missing file for redirection");
 
        if(token == '>')
            temp = redircmd_in2(temp, ptr_file, ptr_endfile, 1);
        else if(token == '<')
            temp = redircmd_out2(temp, ptr_file, ptr_endfile, 0);
        else if(token == '+')
            temp = redircmd_append2(temp, ptr_file, ptr_endfile, 1);
        else if(token == 'h')
        {
            // printf("we balling\n");
            temp = redircmd_here2(pipes, temp);
            // gettoken(ps, 0, 0);
            // printf("we balling2\n");
        }
        else
        {
            problem("this aint a redirection");
            break;
        }
    }
    return (cmd);
}

void findredir(t_cmd *cmd, t_cmd **temp)
{
    *temp = cmd;
    while ((*temp)->type == REDIR || (*temp)->type == HERE)
    {
        if (((t_redircmd *)(*temp))->cmd->type != REDIR && ((t_redircmd *)(*temp))->cmd->type != HERE)
            break ;
        *temp = ((t_redircmd *)(*temp))->cmd;
    }
}



char *other_token(char *temp, int *check)
{
    char a = *temp;

    if(!*temp)
        *check = 0;
    else
        *check = 'a';
    if(a == '\"' || a == '\'')
    {
        temp++;
        while(temp && *temp != a)
            temp++;
        temp++;
        if(*temp == '\'' || *temp == '\"')
            other_token(temp, check);
    }
    else
        while(*temp && !ft_strchr(" \t\n\r\v<>()|&", *temp))
            temp++;
    return (temp);
}

void    extratoken(char **temp, int *check)
{
        if(*check == '|' && *(*temp) == '|')
        {
            (*temp)++;
            *check = 'o';
        }
        else if(*check == '&' && *(*temp) == '&')
        {
            (*temp)++;
            *check = 'a';
        }
        else if(*check == '>' && *(*temp) == '>')
        {
            (*temp)++;
            *check = '+';
        }
        else //if (*check == '<' && *(*temp) == '<') /// heredoc
        {
            (*temp)++;
            *check = 'h';
        }
}

int gettoken(char **ptr, char **ptr_token, char **ptr_endtoken)
{
    char *temp;
    int check;
    
    temp = *ptr;
    while(*temp && ft_strchr(" \t\n\r\v", *temp))   
        temp++;
    if(ptr_token)
        *ptr_token = temp;
    check = *temp;
    if(ft_strchr("<>()|&", check))
    {   
        
        temp++;
        if(check == *temp && check != '(' && check != ')')
            extratoken(&temp, &check); // Checking for ||, &&, >>, <<
    }
    else
        temp = other_token(temp, &check); // \0 or words
    if(ptr_endtoken)
        *ptr_endtoken = temp;
    *ptr = temp;
    return (check);
}

void problem(char *s) // A sauter de partout
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}
t_cmd *parseblock(t_shell *shell, char **ps, int *check)
{
    t_cmd *cmd;
    if(!lfsymbol(ps, "("))
        problem("this aint my block, oopy wrong hood?!?");
    gettoken(ps,  0, 0);
    cmd = parseline(shell, ps);
    if(!lfsymbol(ps, ")"))
        problem("syntax error");
    gettoken(ps, 0, 0);
    // printf("ps: %s\n", *ps);
    return (cmd);
}
void initialize_cmd(t_cmd **ret, t_execcmd **cmd, int *argc) 
{
    *ret = execcmd();
    *cmd = (t_execcmd *)*ret;
    *argc = 0;
}

char *ft_strdup_arg(char *start, char *end)
{
    char *copy;
    int i;
    int size;

    i = 0;
    size = 0;
    while((start + size) != end && start[size] != '\0')
        size++;
    copy = malloc(sizeof(char) * (size + 1));
    if(!copy)
        return (NULL);
    while(i != size)
    {
        copy[i] = start[i];
        i++;
    }
    copy[i] = '\0';
    return (copy);
}
int parse_single_argument(t_execcmd *cmd, char **ps, int argc)
{
    char *ptr_arg;
    char *ptr_earg;
    int token;

    token = gettoken(ps, &ptr_arg, &ptr_earg);
    if (token == 'a') 
    {
        cmd->args[argc] = ft_strdup_arg(ptr_arg, ptr_earg);
        return (1);
    } 
    else if (token != 0)
        problem("syntax");
    return 0;
}

t_cmd *parseexec(t_shell *shell, char **ps) 
{
    t_cmd *ret;
    t_execcmd *cmd;
    int argc;
    int check;
    check = 1;
    if(lfsymbol(ps, "("))
    {
        ret = parse_subshell(shell, ps, &check);
        parseredirs_primo(&shell->pipe, &ret, ps, &check);
        return (ret);
    }
    initialize_cmd(&ret, &cmd, &argc);
    parseredirs_primo(&shell->pipe, &ret, ps, &check);
    while (!lfsymbol(ps, "|)<>&\0")) 
    {
        if (!parse_single_argument(cmd, ps, argc))
            break;
        argc++;
        if (argc >= MAXARGS)
            problem("too many args");
        parseredirs_primo(&shell->pipe, &ret, ps, &check);
    }
    // parseredirs_primo(&temp, ps, es, &check);
    cmd->args[argc] = 0;
    cmd->eargs[argc] = 0;
    return (ret);
}

t_cmd *parse_double_node(t_shell *shell, char **ptr)
{
    t_cmd *cmd;
    char token;

    cmd = parseexec(shell, ptr);
    while(*ptr)
    {
        if (lfsymbol(ptr, "|"))
        {
            if(gettoken(ptr,  0, 0) == 'o')
                cmd = orcmd(cmd, parse_double_node(shell, ptr));
            else
                cmd = pipecmd(cmd, parse_double_node(shell, ptr));
        }
        else if (lfsymbol(ptr, "&"))
        {
            gettoken(ptr, 0, 0);
            cmd = andcmd(cmd, parse_double_node(shell, ptr));
        }
        else
            break;
    }
    return (cmd);
}
t_cmd *parse_and(t_shell *shell, char **ptr)
{
    t_cmd *cmd;

    cmd = parse_double_node(shell, ptr);
    if(*ptr && lfsymbol(ptr,  "&&"))
    {
        gettoken(ptr, 0, 0);
        cmd = andcmd(cmd, parse_and(shell, ptr));
    }
    return (cmd);
}

t_cmd *parse_or(t_shell *shell, char **ptr)
{
    t_cmd *cmd;

    cmd = parse_double_node(shell, ptr);
    if(*ptr  && lfsymbol(ptr, "||"))
    {
        gettoken(ptr,  0, 0);
        cmd = orcmd(cmd, parse_or(shell, ptr));
    }
    return (cmd);
}


t_cmd *parseline(t_shell *shell, char **ptr)
{
    t_cmd *cmd;

    cmd = parse_double_node(shell, ptr);

    return (cmd);
}
void nulargs(t_cmd* cmd)
{
    int i;
    int j;

    i = 0;
    t_execcmd *exe;

    exe = (t_execcmd *)cmd;

    while((exe->args[i]) != 0)
    {
        *exe->eargs[i] = 0;
        i++;
    }
}
void nulredir(t_cmd *cmd)
{
    t_redircmd *redir;

    redir = (t_redircmd *)cmd;
    nulterminator(redir->cmd);
    (*redir->efile) = 0;
    return ;
}
void nulterminator(t_cmd *cmd)
{
    if(cmd->type == PIPE)
    {
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->left);
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->right);
    }
    else if(cmd->type == REDIR)
        nulredir(cmd);
    if(cmd->type == OR)
    {
        nulterminator((t_cmd *)((t_orcmd *)cmd)->left);
        nulterminator((t_cmd *)((t_orcmd *)cmd)->right);
    }
    if(cmd->type == AND)
    {
        nulterminator((t_cmd *)((t_andcmd *)cmd)->left);
        nulterminator((t_cmd *)((t_andcmd *)cmd)->right);
    }
    if(cmd->type == SUB)
    {
        nulterminator ((t_cmd *)((t_sub *)cmd)->cmd);
    }

    return ;
}

// Global return should be 2 when exiting program if it fails

void emptyswitch(t_cmd *cmd)
{
    t_execcmd *ex;
    t_pipecmd *temp;
    t_redircmd *re;

    if(cmd->type == EXEC)
    {
        ex = (t_execcmd *)cmd;
        if(ex->args[0] == 0)
            ex->type = EMPTY;
    }
    else if(cmd->type == REDIR)
    {
        re = (t_redircmd *)cmd;
        if(re->cmd == 0)
            problem("syntax error");
    }
    else if(cmd->type == 3)
    {
        temp = (t_pipecmd *)cmd;
        emptyswitch(temp->left);
        emptyswitch(temp->right);
    }
    else if(cmd->type == 5)
    {
        temp = (t_pipecmd *)cmd;
        emptyswitch(temp->left);
        emptyswitch(temp->right);
    }
    else if(cmd->type == 6)
    {
        temp = (t_pipecmd *)cmd;
        emptyswitch(temp->left);
        emptyswitch(temp->right);
    }
}

void *parsecmd(t_shell **shell, char *str)
{
    t_cmd *cmdtree;

    if(str[0] == '\0')
        return (free(str), NULL);
    if(lexer(shell, str)){printf("LEXER FAILED\n"); 
        return (NULL);}
    (*shell)->tree = parseline(*shell, &str);  
    emptyswitch((*shell)->tree);
    printf("TREE %d\n", (*shell)->tree->type);
    return (0);    
}

void printer(t_cmd *cmd, int s, int level) {
    t_execcmd *ex;
    t_pipecmd *pi;
    t_redircmd *re;
    t_orcmd *or;
    t_andcmd *and;
    t_sub *sub;
    printf("INSIDE PRINTER\n\n");
    // Create indentation based on the current level
    for (int i = 0; i < level; i++) {
    //printf("  ");
    }

    switch (cmd->type) {
        case 1: // EXEC
            ex = (t_execcmd *)cmd;
        //printf("EXEC\n");
            for (int i = 0; ex->args[i] != 0; i++) {
                for (int j = 0; j < level; j++) {
                //printf("  ");
                }
            //printf("args[%d] : %s\n", i, ex->args[i]);
            }
            break;
        
        case 2: // REDIR
            re = (t_redircmd *)cmd;
        //printf("REDIR\n");
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("file : %s\n", re->file);
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("mode : %d\n", re->mode); // Print mode
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("fd : %d\n", re->fd); // Print fd
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("cmd:\n");
            printer(re->cmd, re->type, level + 1); // Recursively print the nested command
            break;

        case 3: // PIPE
            pi = (t_pipecmd *)cmd;
        //printf("PIPE\n");
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("left :\n");
            printer(pi->left, pi->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("right :\n");
            printer(pi->right, pi->type, level + 1); // Increase level for indentation
            break;

        case 5: // AND
            and = (t_andcmd *)cmd;
        //printf("AND\n");
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("left :\n");
            printer(and->left, and->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("right :\n");
            printer(and->right, and->type, level + 1); // Increase level for indentation
            break;

        case 6: // OR
            or = (t_orcmd *)cmd;
        //printf("OR\n");
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("left :\n");
            printer(or->left, or->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("right :\n");
            printer(or->right, or->type, level + 1); // Increase level for indentation
            break;

        case 8: // EMPTY
            ex = (t_execcmd *)cmd;
        //printf("EMPTY\n");
            for (int i = 0; i < level; i++) {
            //printf("  ");
            }
        //printf("args[0] : %s\n", ex->args[0]);
            break;
        //Add a case for SUB
        case 4:
            sub = (t_sub *)cmd;
        //printf("SUB\n");
            printer(sub->cmd, 0, level + 1);
            break;

        default:
        //printf("Unknown command type: %d\n", cmd->type);
            break;
    }
}

void free_args(t_execcmd *ex)
{
    int i;

    i = 0;
    while(ex->args[i] != 0)
    {
        free(ex->args[i]);
        i++;
    }
    free(ex->args[i]);
}


void free_exec(t_execcmd *ex)
{
        free_args(ex);
        free(ex);
}
void free_sub(t_sub *sub)
{
        tree_free(&sub->cmd);
        free(sub);
}
void  free_redir(t_redircmd *re)
{
        tree_free(&re->cmd);
        free(re);
}
void free_double(t_doublecmd *db)
{
    tree_free(&db->left);
    tree_free(&db->right);
    free(db);
}

void tree_free(t_cmd **tree)
{
    t_execcmd *ex;
    t_redircmd *re;
    t_doublecmd *db;
    t_sub *sub;

    if (tree == NULL || *tree == NULL)
        return ;
    if ((*tree)->type == REDIR) 
        free_redir((t_redircmd *)(*tree));
    else if ((*tree)->type == EXEC)
        free_exec((t_execcmd *)(*tree));
    else if ((*tree)->type == SUB)
        free_sub((t_sub *)(*tree));
    else// if ((*tree)->type == PIPE || (*tree)->type == AND || (*tree)->type == OR)
        free_double((t_doublecmd *)(*tree));
}
// PIPEX FOR PATH

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

static char	*ft_strjoin(char const *s1, char const *s2)
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
char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*true_path;
	char	*true_cmd;
	char	**commands;
	char	**all_paths;

	i = -1;
	commands = ft_split(cmd, ' ');
	all_paths = ft_split(get_env("PATH", env), ':');
	if(all_paths)
		while (all_paths[++i])
		{	
			true_path = ft_strjoin(all_paths[i], "/");
			true_cmd = ft_strjoin(true_path, commands[0]);
			free(true_path);
			if (access(true_cmd, F_OK | X_OK) == 0)
				return (free_array(commands), free_array(all_paths), true_cmd);
			free(true_cmd);
		}
	return (free_array(commands), free_array(all_paths), cmd);
}



void ft_exec(t_cmd *tree, char **env)
{
    t_execcmd *ex;
    t_doublecmd *db;
    if(tree->type == EXEC)
    {
        ex = (t_execcmd *)tree;
    //printf("je rentre dans le exec\n");
        printer((t_cmd *)ex, 0, 0);
        execve(get_path(ex->args[0], env), ex->args, NULL);
    }
    if(tree->type == AND || tree->type == OR)
    {
        db = (t_doublecmd *)tree;
        t_cmd *left = db->left;
        t_cmd *right = db->right;
        int a = fork();
        if(a)
            ft_exec(left, env);
        if(waitpid(a, NULL, 0) != 0)
        {
        //printf("je renre ici\n");
            ft_exec(right, env);
            wait(NULL);
        }
        // wait(NULL);
    }

    return ;
}




// echo a > b && echo b > c || ()
// WORD > >> < << | && || ( ) \0


// TOKEN	VALUE	BEFORE	                        AFTER
// \0	    0	    ) WORD 	                        Nothing
// WORD	    1	    > >> && || |  << < ( START	    > >> && || | EOF  << < WORD )
// <	    2	    && || |  ( START	            WORD 
// <<	    2	    && || |  ( START	            WORD 
// >	    3	    && || |  ( ) START	            WORD 
// >>	    3	    && || |  ( ) START	            WORD 
// AND / OR 4	    ) WORD	                        ( WORD < << > >> 
// PIPE	    5	    ) WORD 	                        WORD () && || > >> < <<


/*
    (echo o) echo b
    bash: syntax error near unexpected token `echo'
*/

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen (src);
	if (size <= 0 || !(dst))
		return (len);
	while (i < len && i < size - 1)
		*dst++ = src[i++];
	*dst = 0;
	return (len);
}

t_lexer	*ft_lstlast(t_lexer *lst)
{
	if (!lst)
		return (0);
	while (lst->next != 0)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_lexer **lst, t_lexer **new)
{
	t_lexer	*temp;

	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = (*new);
	else
	{

		temp = *lst;
		temp = ft_lstlast(temp);
        temp->next = (*new);
        (*new)->prev = temp;
	}
}

int add_quote_node(t_lexer **lex, char *str, int *i, char c)
{

    t_lexer *new;
    new = malloc(sizeof(*new));
    if(!new)
        return (1);

    while(str[*i] && !ft_strchr(" \t\n\r\v><|&()", str[*i]))
        (*i)++;

    new->type = LEX_WORD;
    new->next = NULL;
    new->prev = NULL;

    ft_lstadd_back(lex, &new);

    return 0;
}
void skip_quotes(char *str, int *i, int *size)
{
    char temp;
    int start;

    start = *i;
    temp = str[*i];  
    (*i)++;          
    while (str[*i] && str[*i] != temp)
        (*i)++;
    if (str[*i] == temp)
        (*i)++;
    if (str[*i] == '\'' || str[*i] == '\"')
        skip_quotes(str, i, size);
    *size = *i - start;
}

char *ft_strdup_lex(char *src, int size)
{
    int i;
    char *new;

    new = malloc(sizeof(char) * (size + 1));
    if(!new)
        return (NULL);
    i = 0;
    while(i < size)
    {
        new[i] = src[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

int add_lex_node(t_lexer **lex, char *str, int *i)
{
    t_lexer *new;
    int size;
    int before;
    int end;

    size = 0;
    new = malloc(sizeof(*new));
    before = *i;
    if (!new)
        return (1);
    while (str[*i] && (ft_strchr(" \t\n\r\v><|&()", str[*i]) == 0))
    {
        if (str[*i] == '\'' || str[*i] == '\"')
            skip_quotes(str, i, &size);
        else
        {
            (*i)++;  
            size++;
        }
    }
    new->type = LEX_WORD;
    new->next = NULL;
    new->prev = NULL;
    new->heredoc = ft_strdup_lex(str + before, size);
    ft_lstadd_back(lex, &new);
    return 0;
}

void lex_tozero(t_lexer **lex)
{
    (*lex)->type = 0;
    (*lex)->heredoc = 0;
    (*lex)->next = NULL;
    (*lex)->prev = NULL;
    (*lex) = NULL;
}

int lexer(t_shell **shell, char *str)
{
    int i = 0;
    t_lexer *lexer;

    lexer = malloc(sizeof(*lexer));
    if (!lexer)
        return (1);
    ft_bzero(lexer, sizeof(*lexer));
    
    while (str[i])
    {
        while (str[i] && ft_strchr(" \t\n\r\v", str[i]))
            i++;
        if (str[i] && ft_strchr("><|&()", str[i]))
        {
            if (sub_lexer(&lexer, str, &i)) 
                return (free_lexer(lexer), 1);
            continue;
        }
        if (str[i] && !ft_strchr(" \t\n\r\v><|&()", str[i]))
        {
            if (add_lex_node(&lexer, str, &i)) 
                return (free_lexer(lexer), 1);
        }
    }
    if (lexing_check(shell, lexer))
        return (free_lexer(lexer), 1);
    return (free_lexer(lexer), 0);
}

int sub_lexer(t_lexer **lex, char *str, int *i)
{
    t_lexer *new;

    new = malloc(sizeof(*new));
    if (!new)
        return (1);

    if (str[*i] == '>')
    {
        new->type = LEX_REDIRIN;
        if (str[*i + 1] == '>')
        {  
            (*i)++;
            new->type = LEX_APPEND;
        }
    }
    else if (str[*i] == '<')
    {
        new->type = LEX_REDIROUT;
        if (str[*i + 1] == '<')  
        {
            new->type = LEX_HERE;
            (*i)++;
        }
    }
    else if (str[*i] == '(')
        new->type = LEX_OPEN;
    else if (str[*i] == ')')
        new->type = LEX_CLOSE;
    else if (str[*i] == '&')
    {
        new->type = LEX_AND;
        if (str[*i + 1] == '&') 
            (*i)++;
        else
            return (free(new), 1);  
    }
    else if (str[*i] == '|')
    {
        new->type = LEX_PIPE;
        if (str[*i + 1] == '|')
        {
            new->type = LEX_OR;
            (*i)++;
        }
    }

    (*i)++;
    new->heredoc = NULL;
    new->next = NULL;
    new->prev = NULL;
    ft_lstadd_back(lex, &new);
    return 0;
}


int prev_check(t_lexer *lex, char *str)
{
    int i;

    i = 0;
    if(!lex)
        return (1);
    while(str[i])
    {   
        if((lex->prev->type + 48) == str[i])
            return (0);
        if(str[i] == '>' && lex->prev->type == LEX_APPEND)
            return (0);
        i++;
    }
    return (1);
}
int next_check(t_lexer *lex, char *str)
{
    int i;

    i = 0;
    if(!lex)
        return (1);
    while(str[i])
    {
        if(lex->next == 0)
        {
            if(str[i] == '0')
                return (0);
            else
                return (1);
        }
        if((lex->next->type + 48) == str[i])
            return (0);
        i++;
    }
    return (1);
}
int first_type_check(t_lexer *lex, int *i, t_shell **shell)
{
    if(lex->type != LEX_START)
    {
        printf("Minishell: syntax error near unexpected token `newline\'\n");
        return (1);
    }
    // if(lex->type != LEX_WORD && lex->type != LEX_OPEN && lex->type != LEX_HERE && lex->type != LEX_REDIRIN && lex->type != LEX_APPEND)
    // {
    //     type_check(lex, i, shell);
    //     return (1);
    // }
    // if(lex->type == 1)
    //     return (0);
    // if((lex->type == 6 || lex->type == 2 || lex->type == 3) && lex->next == 0)
    // {
    //     printf("Minishell: syntax error near unexpected token `newline\'\n");
    //     return (1);
    // }
    // else //(lex->type == 9)
    // {
        
    //     if(lex->type == 9 && lex->next && lex->next->type == 1)
    //         if(!init_heredoc(lex->next, shell))
    //             return (0);
    //     if(lex->next)
    //         type_check(lex->next, i, shell);
    //     else
    //         printf("Minishell: syntax error near unexpected token `newline\'\n");
    //     return (1);
    // }
    return (0);
}

size_t	ft_strlen_heredoc(const char *str)
{
	size_t	i;

	i = 0;
    if(!str)
        return (0);
	while (str[i])
		i++;
	return (i);
}
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
char	*ft_strjoin_heredoc(char const *s1, char const *s2)
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

char *empty_string()
{
    char *str;

    str = malloc(sizeof(char) * 1);
    if(!str)
        return (NULL);
    str[0] = '\0';
    return (str);
}
char *heredoc_filler(char *end)
{
    char *buf;
    char *heredoc;

	heredoc = NULL;
    buf = (char*)readline("heredoc> ");
    if(ft_strncmp(end, buf ,ft_strlen(end)) == 0)
    {   
        free(buf);
        return (NULL);
    }
    while(ft_strncmp(end, buf ,ft_strlen(end)) != 0)
    {   
        if(buf == NULL)
            return (NULL);
        heredoc = ft_strjoin_heredoc(heredoc, buf);
        free(buf);
        buf = (char*)readline("heredoc> ");
    }
    return (heredoc);
}

// t_herepipe  ft_lastpipe(t_herepipe *pipe)
// {
//     t_herepipe *temp;

//     temp = pipe;
//     while(temp->next)
//         temp = temp->next;
//     return (*temp);
// }

void  ft_pipeaddback(t_shell **shell, t_herepipe *new)
{
    t_herepipe *temp;

    if((*shell)->pipe == NULL)
    {
        (*shell)->pipe = new;
        return ;
    }
    temp = (*shell)->pipe;
    while(temp->next)
        temp = temp->next;
    temp->next = new;
}


// int init_heredoc(t_lexer *lex, t_shell **shell)
// {
//     char *eof;
//     int err;
//     int pipefd[2];
//     t_herepipe *temp;

//     temp = malloc(sizeof(*temp));
//     if(!temp)
//     {
//     //printf("Minishell: malloc failed\n");
//         return (1);
//     }
//     if(pipe(pipefd) == -1)
//     {
//     //printf("Minishell: pipe failed\n");
//         return (1);
//     }
//     eof = lex->next->heredoc;
//     err = heredoc_filler(eof, pipefd[1]);
//     if(err)
//     {
//     //printf("Minishell: syntax error near unexpected token `newline\'\n");
//         return (1);
//     }
//     close(pipefd[1]);
//     temp->stored = 0;
//     temp->fd = pipefd[0];
//     temp->next = NULL;
//     ft_pipeaddback(shell, temp);
//     return (0);
// }


int init_heredoc(t_lexer *lex, t_shell **shell)
{
    char *eof;
    char *temp;
    t_herepipe *node;

    temp = malloc(sizeof(*temp));
    if(!temp)
    {
    //printf("Minishell: malloc failed\n");
        return (1);
    }
    temp = heredoc_filler(lex->next->heredoc);
    node = malloc(sizeof(*node));
    node->str = temp;
    node->stored = 0;
    node->next = NULL;
    ft_pipeaddback(shell, node);
    return (0);
}

int type_check(t_lexer *lex, int *i, t_shell **shell)
{

    if(lex->type == LEX_WORD)
    { // WORD 
        if(prev_check(lex, "012345689>"))
        {
            printf("Lexer : syntax error\n");
            return (1);
        }
    }
    else if(lex->type == LEX_REDIROUT) // <
    {
        if(prev_check(lex, "-0145678"))
        {
            printf("Minishell: syntax error near unexpected token `<'\n");
            return (1);
        }    
    }
    else if(lex->type == LEX_APPEND) // >>
    {
        if(prev_check(lex, "-0145678"))
        {
            printf("Minishell: syntax error near unexpected token `>>'\n");
            return (1);
        }
    }
    else if(lex->type == LEX_REDIRIN) // >
    {
        if(prev_check(lex, "-0145678"))
        {
            printf("Minishell: syntax error near unexpected token `>'\n");
            return (1);
        }
    }
    else if(lex->type == LEX_AND) // AND
    {   
        if(prev_check(lex, "17"))
        {
            printf("Minishell:  syntax error near unexpected token `&&\'\n");
            return (1);
        }    
    }
    else if(lex->type == LEX_PIPE) // PIPE
    {
        if(prev_check(lex, "17"))
        {
            printf("Minishell: syntax error near unexpected token `|\'\n");
            return (1);
        }
    }
    else if(lex->type == LEX_OPEN && ++(*i))
    {
        if(prev_check(lex, "-04568"))
        {
            printf("Minishell: syntax error near unexpected token `(\'\n");
            return (1);
        }
    }
    else if(lex->type == LEX_CLOSE && (*i)-- >= -1) // )
    {
        if(prev_check(lex, "17"))
        {
        printf("Minishell: syntax error near unexpected token `)\'\n");
            return (1);
        }
    }
    else if(lex->type == LEX_OR) // OR
    {
        if(prev_check(lex, "17"))
        {
            printf("Minishell: syntax error near unexpected token ||\n");
            return (1);
        }
    }
    else if(lex->type == LEX_HERE) // <<
    {
        if(prev_check(lex, "0145678") || next_check(lex, "1")) // pas sur la?????
        {
            printf("Minishell: syntax error near unexpected token `<<\'\n");
            return (1);
        }    
        init_heredoc(lex, shell);
    }
    return (0);
}
int last_type_check(int lex)
{
    if(lex != 1 && lex != 7)
    {

        printf("Minishell: syntax error near unexpected token `newline\'\n");
        return (1);
    }
    return (0);
}
int lexing_check(t_shell **shell, t_lexer *lexer)
{
    t_lexer *lex;
    
    int i;

    i = 0;
    lex = lexer;

    if(first_type_check(lex, &i, shell))
        return (1);
    lex = lex->next;
    while(lex)
    {
        if(type_check(lex, &i, shell))
            return (1);
        if(lex->next == NULL)
            if(last_type_check(lex->type))
                return (1);   
        lex = lex->next;
        if(i < 0)
        {
            printf("Minishell: syntax error near unexpected token `)\'\n");
            return (1);
        }
    }
    if(i)
    {
        printf("Minishell: syntax error near unexpected token `(\'\n");
        return (1);
    }
    return (0);
}
void free_lexer(t_lexer *lexer)
{
    t_lexer *lex;

    while (lexer)
    {
        lex = lexer->next;
        if (lexer->heredoc)
            free(lexer->heredoc);
        free(lexer);
        lexer = lex;
    }
}



void print_cmd(t_cmd *cmd, int indent);

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void print_redircmd(t_redircmd *redir, int indent) {
    print_indent(indent);
    printf("Redirection Command:\n");
    print_indent(indent + 1);
    printf("Type: %s\n", redir->type == REDIR ? "REDIR" : "HERE");
    print_indent(indent + 1);
    printf("File: %s\n", redir->file);
    print_indent(indent + 1);
    printf("STR: %s\n", redir->heredoc); // Added newline character
    print_indent(indent + 1);
    printf("Mode: %d\n", redir->mode);
    print_indent(indent + 1);
    printf("FD: %d\n", redir->fd);
    print_cmd(redir->cmd, indent + 1);
}

void print_execcmd(t_execcmd *exec, int indent) {
    print_indent(indent);
    printf("Execution Command:\n");
    for (int i = 0; exec->args[i]; i++) {
        print_indent(indent + 1);
        printf("Arg[%d]: %s\n", i, exec->args[i]);
    }
}

void print_pipecmd(t_pipecmd *pipe, int indent) {
    print_indent(indent);
    printf("Pipe Command:\n");
    print_indent(indent + 1);
    printf("Left:\n");
    print_cmd(pipe->left, indent + 2);
    print_indent(indent + 1);
    printf("Right:\n");
    print_cmd(pipe->right, indent + 2);
}

void print_andcmd(t_andcmd *and, int indent) {
    print_indent(indent);
    printf("AND Command:\n");
    print_indent(indent + 1);
    printf("Left:\n");
    print_cmd(and->left, indent + 2);
    print_indent(indent + 1);
    printf("Right:\n");
    print_cmd(and->right, indent + 2);
}

void print_orcmd(t_orcmd *or, int indent) {
    print_indent(indent);
    printf("OR Command:\n");
    print_indent(indent + 1);
    printf("Left:\n");
    print_cmd(or->left, indent + 2);
    print_indent(indent + 1);
    printf("Right:\n");
    print_cmd(or->right, indent + 2);
}

void print_doublecmd(t_doublecmd *dbl, int indent) {
    print_indent(indent);
    printf("Double Command:\n");
    print_indent(indent + 1);
    printf("Left:\n");
    print_cmd(dbl->left, indent + 2);
    print_indent(indent + 1);
    printf("Right:\n");
    print_cmd(dbl->right, indent + 2);
}

void print_subcmd(t_sub *sub, int indent) {
    print_indent(indent);
    printf("Sub Command:\n");
    print_cmd(sub->cmd, indent + 1);
}

void print_cmd(t_cmd *cmd, int indent) {

    if (!cmd) return;

    switch (cmd->type) {
        case SHELL:
            print_indent(indent);
            printf("Shell Command\n");
            break;
        case EXEC:
            print_execcmd((t_execcmd *)cmd, indent);
            break;
        case REDIR:
        case HERE:
            print_redircmd((t_redircmd *)cmd, indent);
            break;
        case PIPE:
            print_pipecmd((t_pipecmd *)cmd, indent);
            break;
        case AND:
            print_andcmd((t_andcmd *)cmd, indent);
            break;
        case OR:
            print_orcmd((t_orcmd *)cmd, indent);
            break;
        case SUB:
            print_subcmd((t_sub *)cmd, indent);
            break;
        case EMPTY:
            print_indent(indent);
            printf("Empty Command\n");
            break;
        default:
            print_indent(indent);
            printf("Unknown Command Type\n");
            break;
    }
}

void free_cmd(t_cmd *cmd);

void free_var(t_var *var)
{
    t_var *temp;

    while (var)
    {
        temp = var->next;
        free(var->alias);
        free(var->value);
        free(var);
        var = temp;
    }
}

void free_herepipe(t_herepipe *pipe)
{
    t_herepipe *temp;

    while (pipe)
    {
        temp = pipe->next;
        free(pipe->str);
        free(pipe);
        pipe = temp;
    }
}

void free_env(t_env *env)
{
    if (!env || !env->env)
        return;

    for (int i = 0; env->env[i]; i++)
    {
        free(env->env[i]);
    }
    free(env->env);
    free(env);
}

void free_cmd(t_cmd *cmd)
{
    if (!cmd)
        return;

    // Depending on the type, cast to the appropriate command type and free
    switch (cmd->type)
    {
        case 1:  // EXEC
            {
                t_execcmd *ecmd = (t_execcmd *)cmd;
                for (int i = 0; i < MAXARGS && ecmd->args[i]; i++)
                {
                    free(ecmd->args[i]);
                    free(ecmd->eargs[i]);
                }
                free(ecmd);
            }
            break;
        case 2:  // PIPE
            {
                t_pipecmd *pcmd = (t_pipecmd *)cmd;
                free_cmd(pcmd->left);
                free_cmd(pcmd->right);
                free(pcmd);
            }
            break;
        case 3:  // REDIR
            {
                t_redircmd *rcmd = (t_redircmd *)cmd;
                free(rcmd->file);
                free(rcmd->efile);
                free_cmd(rcmd->cmd);
                free(rcmd);
            }
            break;
        case 4:  // AND
            {
                t_andcmd *acmd = (t_andcmd *)cmd;
                free_cmd(acmd->left);
                free_cmd(acmd->right);
                free(acmd);
            }
            break;
        case 5:  // OR
            {
                t_orcmd *ocmd = (t_orcmd *)cmd;
                free_cmd(ocmd->left);
                free_cmd(ocmd->right);
                free(ocmd);
            }
            break;
        case 6:  // DOUBLE
            {
                t_doublecmd *dcmd = (t_doublecmd *)cmd;
                free_cmd(dcmd->left);
                free_cmd(dcmd->right);
                free(dcmd);
            }
            break;
        case 7:  // SUBSHELL
            {
                t_sub *scmd = (t_sub *)cmd;
                free_cmd(scmd->cmd);
                free(scmd);
            }
            break;
        case 8:  // HERE
            {
                t_herecmd *hcmd = (t_herecmd *)cmd;
                free_cmd(hcmd->cmd);
                free(hcmd);
            }
            break;
        default:
            free(cmd);
            break;
    }
}

void free_shell(t_shell *shell)
{
    if (!shell)
        return;

    free_cmd(shell->tree);  // Free the command tree
    free_var(shell->var);   // Free the variables
    free_herepipe(shell->pipe);  // Free the here-doc pipes
    free_env(shell->env);    // Free the environment

    free(shell);  // Finally, free the shell itself
}


int main(int ac, char **av, char **env)
{
    t_cmd *tree;
    t_lexer *lex;
    t_shell *shell;

    shell = malloc(sizeof(t_shell));
    ft_bzero(shell, sizeof(t_shell));
    char *copy;

    copy = strdup(av[1]);
    parsecmd(&shell ,copy);
    print_cmd(shell->tree, 0);
    free(copy);
    free(shell); // If lexer failed shell empty, if not plenty more to free
    // free_shell(shell);

    // free(shell);
    // tree_free(&shell->tree);
    //     // free(copy);
    //     return (0);
    // }
    // free(copy);
    // ft_exec(tree, env);
    return (1);
}