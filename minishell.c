
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// #define EXEC 1
// #define REDIR 2
// #define PIPE 3
// #define BLOC 4
// #define AND 5
// #define OR 6
// #define HERE 7
// #define EMPTY 8


#define MAXARGS 50
#define MAXLINE 1500001



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
    t_cmd *sub;
}   t_sub;

typedef struct s_var
{
    char *alias;
    char *value;
    struct s_var *next;
}   t_var;

typedef struct s_shell
{
    int type;
    t_cmd *tree;
    t_var *var;
    t_env *env;
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
    char *here;
    char *ehere;
}   t_herecmd;

typedef struct s_herepipe
{
    int fd;
    int empty;
    struct s_herepipe *next;
} t_herepipe;

t_cmd *parse_and(char **ptr, char *end);
t_cmd *parse_or(char **ptr, char *end);
void nulterminator(t_cmd *cmd);
void nulargs(t_cmd *cmd);
void nulredir(t_cmd *cmd);
void ft_memset(void *s, int c, size_t n);
int ft_strlen(const char *s);
char *ft_strchr(char *s, char c);
int lfsymbol(char **ptr, char *end, char *c);
t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *execcmd(void);
t_cmd *pipecmd(t_cmd *left, t_cmd *right);
t_cmd *parseredirs_er(t_cmd *cmd, char **ps, char *es);
void    parseredirs_primo(t_cmd **cmd, char **ps, char *es, int *check);
int gettoken(char **ptr, char *end, char **ptr_token, char **ptr_endtoken);
void problem(char *s);
t_cmd *parseblock(char **ps, char *es, int *check);
t_cmd *parseexec(char **ps, char *es);
t_cmd *parse_double_node(char **ptr, char *end);
t_cmd *parseline(char **ptr, char *end);
t_cmd *parsecmd(char *str);
t_cmd *parse_all_redirections(t_cmd *cmd, char **ps, char *es);
int parse_single_argument(t_execcmd *cmd, char **ps, char *es, int argc);
void initialize_cmd(t_cmd **ret, t_execcmd **cmd, int *argc);

// Pipex 
void ft_exec(t_cmd *tree, char **env);
char	*get_path(char *cmd, char **env);
static char	*ft_strjoin(char const *s1, char const *s2);
char	*get_env(char *name, char **env);
static char	*ft_substr(char const *s, unsigned int start, size_t len);
void	free_array(char **s);

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

void ft_memset(void *s, int c, size_t n)
{
    unsigned char *p;

    p = s;
    while(n--)
        *p++ = (unsigned char)c;
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


int lfsymbol(char **ptr, char *end, char *c)
{
    char *temp;

    temp = *ptr;
    while(temp < end && ft_strchr(" \t\n\r\v", *temp))
        temp++;
    *ptr = temp;
    return (*ptr && ft_strchr(c, *temp));
}


// Constructors

t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = file;
    redir->efile = efile;
    redir->mode = O_WRONLY | O_CREAT | O_TRUNC;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = file;
    redir->efile = efile;
    redir->mode = O_RDONLY;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;
    redir = malloc(sizeof(*redir));
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->cmd = cmd;
    redir->file = file;
    redir->efile = efile;
    redir->mode = O_WRONLY | O_CREAT | O_APPEND;
    redir->fd = fd;
    return((t_cmd *)redir);
}

t_cmd *redircmd_in2(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;

    redir = malloc(sizeof(*redir));
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->file = file;
    redir->efile = efile;
    redir->mode = O_WRONLY | O_CREAT | O_TRUNC;
    redir->fd = fd;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return((t_cmd *)redir);
}

t_cmd *redircmd_out2(t_cmd *cmd, char *file, char *efile, int fd)
{
    t_redircmd *redir;
    t_redircmd *temp;

    temp = (t_redircmd *)((t_redircmd *)cmd)->cmd;
    redir = malloc(sizeof(*redir));
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->file = file;
    redir->efile = efile;
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
    ft_memset(redir, 0, sizeof(*redir));
    redir->type = REDIR;
    redir->file = file;
    redir->efile = efile;
    redir->mode = O_WRONLY | O_CREAT | O_APPEND;
    redir->fd = fd;
    redir->cmd = ((t_redircmd *)cmd)->cmd;
    ((t_redircmd *)cmd)->cmd = (t_cmd *)redir;
    return((t_cmd *)redir);
}

t_cmd *execcmd(void)
{
    t_execcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_memset(cmd, 0, sizeof(*cmd));
    cmd->type = EXEC;
    return ((t_cmd *)cmd);
};

t_cmd *pipecmd(t_cmd *left, t_cmd *right)
{
    t_pipecmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_memset(cmd, 0, sizeof(*cmd));
    cmd->type = PIPE;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}


t_cmd *andcmd(t_cmd *left, t_cmd *right)
{
    t_andcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_memset(cmd, 0, sizeof(*cmd));
    cmd->type = AND;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}

t_cmd *orcmd(t_cmd *left, t_cmd *right)
{
    t_orcmd *cmd;

    cmd = malloc(sizeof(*cmd));
    ft_memset(cmd, 0, sizeof(*cmd));
    cmd->type = OR;
    cmd->left = left;
    cmd->right = right;
    return ((t_cmd *)cmd);
}

t_cmd *parse_subshell(char **ps, char *es, int *check) 
{
    t_sub *ret;
    ret = malloc(sizeof(*ret));
    ret->type = SUB;
    ret->sub = parseblock(ps, es, check);
    return ((t_cmd *)ret);
}
// Parsing functions
////////////// A CONTINUER ICI SUR PARSEDIR ET FAIRE LE GETTOKEN //////////////////////
//////////////////////////////////////////////////////////////////////////////////////

t_cmd *parseredirs_er(t_cmd *cmd, char **ps, char *es)
{
    char *ptr_file;
    char *ptr_endfile;
    int token;
    t_cmd *temp;

    temp = cmd;
    while(lfsymbol(ps, es, "><"))
    {
        token = gettoken(ps, es, &ptr_file, &ptr_endfile);
        if(gettoken(ps, es, &ptr_file, &ptr_endfile) != 'a')
            problem("missing file for redirection");
 
        if(token == '>')
            temp = redircmd_in2(temp, ptr_file, ptr_endfile, 1);
        else if(token == '<')
            temp = redircmd_out2(temp, ptr_file, ptr_endfile, 0);
        else if(token == '+')
            temp = redircmd_append2(temp, ptr_file, ptr_endfile, 1);
        else if(token == 'h')
            printf("NEED TO IMPLEMENT HERE DOC\n");
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
    while ((*temp)->type == REDIR)
    {
        if (((t_redircmd *)(*temp))->cmd->type != REDIR)
            break ;
        *temp = ((t_redircmd *)(*temp))->cmd;
    }
}

void    parseredirs_primo(t_cmd **cmd, char **ps, char *es, int *check)
{
    char *ptr_file;
    char *ptr_endfile;
    int token;
    t_cmd *temp;

    findredir(*cmd, &temp);
    if (*check && lfsymbol(ps, es, "<>"))
    {
        *check = 0;
        token = gettoken(ps, es, &ptr_file, &ptr_endfile);
        if(gettoken(ps, es, &ptr_file, &ptr_endfile) != 'a')
            problem("missing file for redirection");
        if(token == '>')
            *cmd = redircmd_in(*cmd, ptr_file, ptr_endfile, 1);
        else if(token == '<')
            *cmd = redircmd_out(*cmd, ptr_file, ptr_endfile, 0);
        else if(token == '+')
            *cmd = redircmd_append(*cmd, ptr_file, ptr_endfile, 1);
        else if(token == 'h')
            printf("NEED TO IMPLEMENT HERE DOC\n");
        else
            problem("this aint a redirection");
        parseredirs_er(*cmd, ps, es);
    }
    else
        parseredirs_er(temp, ps, es);
}
void    other_token(char **temp, int *check, char *end)
{
    if(*(*temp) == '\0')
        *check = 0;
    else
        *check = 'a';
    while((*temp) < end && !ft_strchr(" \t\n\r\v<>()|&", *(*temp)))
        (*temp)++;
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
void quote_ptr(char **temp, int *check, char *end)
{
    int quote;
    int nbr;

    quote = *check;
    nbr = 1;
    (*temp)++;
    while((*temp) < end  && nbr != 0)
    {
        if(*(*temp) == quote)
            nbr--;
        (*temp)++;
    }

    *check = 'a';
}
int gettoken(char **ptr, char *end, char **ptr_token, char **ptr_endtoken)
{
    char *temp;
    int check;
    
    temp = *ptr;
    while(temp < end && ft_strchr(" \t\n\r\v", *temp))
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
    {
        if(check == '\'' || check == '\"')
            quote_ptr(&temp, &check, end);
        else 
            other_token(&temp, &check, end); // \0 or words
    }
    if(ptr_endtoken)
        *ptr_endtoken = temp;
    *ptr = temp;
    return (check);
}

void problem(char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}
t_cmd *parseblock(char **ps, char *es, int *check)
{
    t_cmd *cmd;
    if(!lfsymbol(ps, es, "("))
        problem("this aint my block, oopy wrong hood?!?");
    gettoken(ps, es, 0, 0);
    cmd = parseline(ps, es);
    if(!lfsymbol(ps, es, ")"))
        problem("syntax error");
    gettoken(ps, es, 0, 0);
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
        problem("malloc failed");
    while(i != size)
    {
        copy[i] = start[i];
        i++;
    }
    copy[i] = '\0';
    return (copy);
}
int parse_single_argument(t_execcmd *cmd, char **ps, char *es, int argc)
{
    char *ptr_arg;
    char *ptr_earg;
    int token;

    token = gettoken(ps, es, &ptr_arg, &ptr_earg);
    if (token == 'a') 
    {
        cmd->args[argc] = ft_strdup_arg(ptr_arg, ptr_earg);
        return (1);
    } 
    else if (token != 0)
        problem("syntax");
    return 0;
}

t_cmd *parseexec(char **ps, char *es) 
{
    t_cmd *ret;
    t_execcmd *cmd;
    int argc;
    int check;

    check = 1;
    if(lfsymbol(ps, es, "("))
    {
        ret = parse_subshell(ps, es, &check);
        parseredirs_primo(&ret, ps, es, &check);
        return (ret);
    }
    initialize_cmd(&ret, &cmd, &argc);
    parseredirs_primo(&ret, ps, es, &check);
    //fd 
    while (!lfsymbol(ps, es, "|)<>&")) 
    {
        
        if (!parse_single_argument(cmd, ps, es, argc))
            break;
        argc++;
        if (argc >= MAXARGS)
            problem("too many args");
        parseredirs_primo(&ret, ps, es, &check);
    }
    // parseredirs_primo(&temp, ps, es, &check);
    cmd->args[argc] = 0;
    cmd->eargs[argc] = 0;
    return (ret);
}
// t_cmd *parseexec(char **ps, char *es)
// {
//     char *ptr_arg;
//     char *ptr_earg;    // q: start of token, eq: end of token
//     int token;
//     int argc;    // tok: token type, argc: argument count
//     t_execcmd *cmd;   // cmd: command
//     t_cmd *ret;       // ret: return value

//     if(lfsymbol(ps, es, "("))     // if the next token is '('
//         return (parseblock(ps, es));   // calls parseblock and returns the cmd        
//     ret = execcmd();  // creates a new command
//     cmd = (t_execcmd *)ret;                // cast the command to execcmd
//     argc = 0;               // initialize argument count
//     ret = parseredirs(ret, ps, es); // parse redirections
//     while(!lfsymbol(ps, es, "|)<>&"))// while the next token is not a pipe, background, or semicolon
//     {
//         if((token = gettoken(ps, es, &ptr_arg, &ptr_earg)) == 0)
//             break;
//         printf("ps: %s \t Token : %c\n", *ps, token);
//         if(!token)
//             break; // if the token is 0, break
//         if(token == 'a') // if the token is not an argument
//     {
//         cmd->args[argc] = ptr_arg; // sets the argument
//         cmd->eargs[argc] = ptr_earg; // sets the end of the argument
//         argc++; // increment the argument count
//         if(argc >= MAXARGS) // if the argument count is greater than or equal to the maximum number of arguments
//             problem("too many args"); // problem
//     }
//         else
//             problem("syntax"); // pb 
//         ret = parseredirs(ret, ps, es); // parse redirections
//     }
//     ret = parseredirs(ret, ps, es); // parse redirections
//     cmd->args[argc] = 0; // set the last argument to 0
//     cmd->eargs[argc] = 0; // set the end of the last argument to 0
//     return (ret);
// }    

t_cmd *parse_double_node(char **ptr, char *end)
{
    t_cmd *cmd;
    char token;

    cmd = parseexec(ptr, end);
    while(*ptr < end)
    {
        if (lfsymbol(ptr, end, "|"))
        {
            if(gettoken(ptr, end, 0, 0) == 'o')
                cmd = orcmd(cmd, parse_double_node(ptr, end));
            else
                cmd = pipecmd(cmd, parse_double_node(ptr, end));
        }
        else if (lfsymbol(ptr, end, "&"))
        {
            gettoken(ptr, end, 0, 0);
            cmd = andcmd(cmd, parse_double_node(ptr, end));
        }
        else
            break;
    }
    return (cmd);
}

t_cmd *parse_and(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parse_double_node(ptr, end);
    if(*ptr < end && lfsymbol(ptr, end, "&&"))
    {
        gettoken(ptr, end, 0, 0);
        cmd = andcmd(cmd, parse_and(ptr, end));
    }
    return (cmd);
}

t_cmd *parse_or(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parse_double_node(ptr, end);
    if(*ptr < end && lfsymbol(ptr, end, "||"))
    {
        gettoken(ptr, end, 0, 0);
        cmd = orcmd(cmd, parse_or(ptr, end));
    }
    return (cmd);
}


t_cmd *parseline(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parse_double_node(ptr, end);

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
        printf("args[%d] : %s\n", i, exe->args[i]);
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
    // else if(cmd->type == EXEC)
    //     nulargs(cmd);   
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
        nulterminator ((t_cmd *)((t_sub *)cmd)->sub);
    }

    return ;
}

// Global return should be 2 when exiting program if it fails
int checkblock(char *str)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
    {
        if(str[i] == '(' && str[i + 1] == ')')
            return (0);
        if(str[i] == '(')
            j++;
        if(str[i] == ')')
            j--;
        if(j < 0)
            return (0);   
        i++;
    }
    return (j == 0);
}
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
t_cmd *parsecmd(char *str)
{
    t_cmd *cmdtree;
    char *end;

    if(!checkblock(str)) // parse des () pour eviter de free apres tout l'arbre une fois parseline lance
        problem("syntax error");
    end = str + ft_strlen(str);
    cmdtree = parseline(&str, end);
    // A enlever a la fin FPRINTF pour debug
    if(str != end)
        fprintf(stderr, "leftover data: %s\n", str);
    nulterminator(cmdtree);
    emptyswitch(cmdtree);
    return (cmdtree);
}


void printer(t_cmd *cmd, int s, int level) {
    t_execcmd *ex;
    t_pipecmd *pi;
    t_redircmd *re;
    t_orcmd *or;
    t_andcmd *and;
    t_sub *sub;

    // Create indentation based on the current level
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // Print the command based on its type
    switch (cmd->type) {
        case 1: // EXEC
            ex = (t_execcmd *)cmd;
            printf("EXEC\n");
            for (int i = 0; ex->args[i] != 0; i++) {
                for (int j = 0; j < level; j++) {
                    printf("  ");
                }
                printf("args[%d] : %s\n", i, ex->args[i]);
            }
            break;
        
        case 2: // REDIR
            re = (t_redircmd *)cmd;
            printf("REDIR\n");
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("file : %s\n", re->file);
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("mode : %d\n", re->mode); // Print mode
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("fd : %d\n", re->fd); // Print fd
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("cmd:\n");
            printer(re->cmd, re->type, level + 1); // Recursively print the nested command
            break;

        case 3: // PIPE
            pi = (t_pipecmd *)cmd;
            printf("PIPE\n");
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("left :\n");
            printer(pi->left, pi->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("right :\n");
            printer(pi->right, pi->type, level + 1); // Increase level for indentation
            break;

        case 5: // AND
            and = (t_andcmd *)cmd;
            printf("AND\n");
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("left :\n");
            printer(and->left, and->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("right :\n");
            printer(and->right, and->type, level + 1); // Increase level for indentation
            break;

        case 6: // OR
            or = (t_orcmd *)cmd;
            printf("OR\n");
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("left :\n");
            printer(or->left, or->type, level + 1); // Increase level for indentation
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("right :\n");
            printer(or->right, or->type, level + 1); // Increase level for indentation
            break;

        case 8: // EMPTY
            ex = (t_execcmd *)cmd;
            printf("EMPTY\n");
            for (int i = 0; i < level; i++) {
                printf("  ");
            }
            printf("args[0] : %s\n", ex->args[0]);
            break;
        //Add a case for SUB
        case 4:
            sub = (t_sub *)cmd;
            printf("SUB\n");
            printer(sub->sub, 0, level + 1);
            break;

        default:
            printf("Unknown command type: %d\n", cmd->type);
            break;
    }
}

void tree_free(t_cmd **tree)
{
    t_execcmd *ex;
    t_redircmd *re;
    t_doublecmd *db;

    if (tree == NULL || *tree == NULL)
        return;
    if ((*tree)->type == REDIR) 
    {
        re = (t_redircmd *)(*tree);
        if(re->cmd->type == EXEC)
            free(re->cmd);
        else
            tree_free(&re->cmd);
        free(re);
    }
    else if ((*tree)->type == EXEC)
    {
        ex = (t_execcmd *)(*tree);
        free(ex);
    }
    else
    {
        db = (t_doublecmd *)(*tree);
        return(tree_free(&db->left), tree_free(&db->right), free(db));
    }
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
        printf("je rentre dans le exec\n");
        printer((t_cmd *)ex, 0, 0);
        // execve(ex->args[0], ex->args, env);
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
            printf("je renre ici\n");
            ft_exec(right, env);
            wait(NULL);
        }
        // wait(NULL);
    }

    return ;
}
int main(int ac, char **av, char **env)
{
    // char *buf = readline("Gimme something > ");
    t_cmd *tree;
    tree = parsecmd(av[1]);
    printer(tree, 0, 0);
    // ft_exec(tree, env);
    tree_free(&tree);
}