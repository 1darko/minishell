/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:03:36 by dakojic           #+#    #+#             */
/*   Updated: 2024/07/22 16:46:56 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define EXEC 1
#define REDIR 2
#define PIPE 3
#define BLOC 4
#define AND 5
#define OR 6
#define HERE 7
#define EMPTY 8


#define MAXARGS 12
#define MAXLINE 4096



// Structs 
typedef struct s_cmd
{
    int type;
}   t_cmd;

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

// ??????????????? A VOIR ?????
typedef struct s_herecmd
{
    int type;
    t_cmd *cmd;
    char *here;
    char *ehere;
}   t_herecmd;

t_cmd *parseand(char **ptr, char *end);
t_cmd *parseor(char **ptr, char *end);
void nulterminator(t_cmd *cmd);
void nulargs(t_cmd *cmd);
void nulredir(t_cmd *cmd);
void ft_memset(void *s, int c, size_t n);
int ft_strlen(char *s);
char *ft_strchr(char *s, char c);
int lfsymbol(char **ptr, char *end, char *c);
t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *execcmd(void);
t_cmd *pipecmd(t_cmd *left, t_cmd *right);
t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es);
int gettoken(char **ptr, char *end, char **ptr_token, char **ptr_endtoken);
void problem(char *s);
t_cmd *parseblock(char **ps, char *es);
t_cmd *parseexec(char **ps, char *es);
t_cmd *parsepipe(char **ptr, char *end);
t_cmd *parseline(char **ptr, char *end);
t_cmd *parsecmd(char *str);
// Libft needed functions

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

int ft_strlen(char *s)
{
    int i;

    i = 0;
    while(s[i])
        i++;
    return (i);
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
    printf("ON APPEND\n\n");
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

// Parsing functions
////////////// A CONTINUER ICI SUR PARSEDIR ET FAIRE LE GETTOKEN //////////////////////
//////////////////////////////////////////////////////////////////////////////////////

t_cmd *parseredirs(t_cmd *cmd, char **ps, char *es)
{
    char *ptr_file;
    char *ptr_endfile;
    int token;
    // printf("Je suis dans parseredirs\n");
    // printf("Je suis sur %s\n", *ps);
    while(lfsymbol(ps, es, "><"))
    {
        // printf("Je suis sur %s\n", *ps);
        token = gettoken(ps, es, &ptr_file, &ptr_endfile);
        if(gettoken(ps, es, &ptr_file, &ptr_endfile) != 'a')
            problem("missing file for redirection");
        printf("TOKEN EST : %c\n", token);
        printf("cmd est : %s\n", ((t_execcmd *)cmd)->args[0]);
        printf("file est : %s\n", ptr_file);
        if(token == '>')
            cmd = redircmd_out(cmd, ptr_file, ptr_endfile, 0);
        else if(token == '<')  // Maybe pour le + faire un autre redir et le < mettre O_WRONLY|O_CREAT|O_TRUNC
            cmd = redircmd_in(cmd, ptr_file, ptr_endfile, 1);
        else if(token == '+')
            cmd = redircmd_append(cmd, ptr_file, ptr_endfile, 1);
        else
            problem("this aint a redirection");
        break;
    }
    return (cmd);
}

// Ne supporte pas || ni &&
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
    if(check == '<' || check == '(' || check == ')' || check == '|' || check == '&' || check == '>')
    {    
        temp++;
        
        if(*temp == '|')
        {
            temp++;
            check = 'o';
        }
        if(check == '&')
        {
            temp++;
            check = 'a';
        }
    }
    else
    {
        if(*temp == '\0')
            check = 0;
        else
        check = 'a';
        while(temp < end && !ft_strchr(" \t\n\r\v<>()|&", *temp))
            temp++;
    }
    if(*temp == '>')
    {
        temp++;
        check = '+';
    } 
    if(ptr_endtoken)
        *ptr_endtoken = temp;
    *ptr = temp;
    return (check);
}
// A implementer et faire en sorte qu'il free tous les mallocs
void problem(char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}
t_cmd *parseblock(char **ps, char *es)
{
    t_cmd *cmd;
    // printf("je suis ici\n");
    if(!lfsymbol(ps, es, "("))
        problem("this aint my block, oopy wrong hood?!?");
    gettoken(ps, es, 0, 0);
    // printf("je suis a %s\n", *ps);
    cmd = parseline(ps, es);
    // if(!lfsymbol(ps, es, ")"))
    //     problem("Missing ) block aint closed");
    gettoken(ps, es, 0, 0);
    cmd = parseredirs(cmd, ps, es);
    return (cmd);
}
t_cmd *parseexec(char **ps, char *es)
{
    char *ptr_arg;
    char *ptr_earg;    // q: start of token, eq: end of token
    int token;
    int argc;    // tok: token type, argc: argument count
    t_execcmd *cmd;   // cmd: command
    t_cmd *ret;       // ret: return value

    if(lfsymbol(ps, es, "("))     // if the next token is '('
        return (parseblock(ps, es));   // calls parseblock and returns the cmd        
    ret = execcmd();  // creates a new command
    cmd = (t_execcmd *)ret;                // cast the command to execcmd
    argc = 0;               // initialize argument count
    ret = parseredirs(ret, ps, es); // parse redirections
    while(!lfsymbol(ps, es, "|)<>&"))// while the next token is not a pipe, background, or semicolon
    {

        token = gettoken(ps, es, &ptr_arg, &ptr_earg); // get the next token
        // printf("TOKEN EST : %c\n", token);
        // printf("ps: %s\n", ptr_arg);
        // printf("token : %c\n", token);
        if(!token)
            break; // if the token is 0, break
        if(token != 'a') // if the token is not an argument
            problem("syntax"); // pb 
        cmd->args[argc] = ptr_arg; // sets the argument
        cmd->eargs[argc] = ptr_earg; // sets the end of the argument
        argc++; // increment the argument count
        if(argc >= MAXARGS) // if the argument count is greater than or equal to the maximum number of arguments
            problem("too many args"); // problem
    }
    ret = parseredirs(ret, ps, es); // parse redirections
    cmd->args[argc] = 0; // set the last argument to 0
    cmd->eargs[argc] = 0; // set the end of the last argument to 0
    // int x = 0;
    // while(x <= argc)
    // {
    //     printf("Je sors ici et dans cmd->args il y a : %s\n", cmd->args[x]);
    //     x++;
    // }
    return (ret);
}    

t_cmd *parsepipe(char **ptr, char *end)
{
    t_cmd *cmd;
    char token;

    cmd = parseexec(ptr, end);
    while(*ptr < end)
    {
        printf("Je suis dans parsepipe\n");
        if (lfsymbol(ptr, end, "|"))
        {
            if(gettoken(ptr, end, 0, 0) == 'o'){printf("JE RENTRE ICI ||\n");
                cmd = orcmd(cmd, parsepipe(ptr, end));}
            else
                cmd = pipecmd(cmd, parsepipe(ptr, end));
        }
        else if (lfsymbol(ptr, end, "&"))
        {
            printf("JE RENTRE ICI\n");
            gettoken(ptr, end, 0, 0);
            printf("Avant de renter dans andcmd le type est : %d\n", cmd->type);
            cmd = andcmd(cmd, parsepipe(ptr, end));
            printf("En sortant apres le andcmd le type est : %d\n", cmd->type);
        }
        else
            break;
    }
    return (cmd);
}

t_cmd *parseand(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parsepipe(ptr, end);
    if(*ptr < end && lfsymbol(ptr, end, "&&"))
    {
        gettoken(ptr, end, 0, 0);
        cmd = andcmd(cmd, parseand(ptr, end));
    }
    return (cmd);
}

t_cmd *parseor(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parsepipe(ptr, end);
    if(*ptr < end && lfsymbol(ptr, end, "||"))
    {
        gettoken(ptr, end, 0, 0);
        cmd = orcmd(cmd, parseor(ptr, end));
    }
    return (cmd);
}


t_cmd *parseline(char **ptr, char *end)
{
    t_cmd *cmd;

    cmd = parsepipe(ptr, end);

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
    else if(cmd->type == EXEC)
        nulargs(cmd);   
    if(cmd->type == OR)
    {
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->left);
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->right);
    }
    if(cmd->type == AND)
    {
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->left);
        nulterminator((t_cmd *)((t_pipecmd *)cmd)->right);
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

    if(cmd->type == 1)
    {
        ex = (t_execcmd *)cmd;
        if(ex->args[0] == 0)
            ex->type = EMPTY;
    }
    else if(cmd->type == 2)
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
    printf("JE SUIS ICI\n");
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
    // Créer une indentation basée sur le niveau actuel
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    // printf("J'ai été appelé par : %d\n", s);

    if (cmd->type == 1) {
        ex = (t_execcmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("EXEC\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("args[0] : %s\n", ex->args[0]);
        printf("args[1] : %s\n", ex->args[1]);
        printf("args[2] : %s\n", ex->args[2]);
        printf("args[3] : %s\n", ex->args[3]);

    } 
    else if (cmd->type == 2) // PRINT REDIR
    {
        re = (t_redircmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("REDIR\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("file : %s\n", re->file);
        for (int i = 0; i < level; i++) {
            printf("  ");}
        printf("cmd : %s\n", ((t_execcmd*)((t_redircmd*)re)->cmd)->args[0]);
        printf("arg1 : %s\n", ((t_execcmd*)((t_redircmd*)re)->cmd)->args[1]);
    } 
    else if (cmd->type == 3) // PRINT PIPE
    {
        pi = (t_pipecmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("PIPE\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("left : %d\n", pi->left->type);
        printer(pi->left, pi->type, level + 1); // Augmenter le niveau pour l'indentation
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("right : %d\n", pi->right->type);
        printer(pi->right, pi->type, level + 1); // Augmenter le niveau pour l'indentation
    } else if (cmd->type == 5) {
        and = (t_andcmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("AND\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("left : %d\n", and->left->type);
        printer(and->left, and->type, level + 1); // Augmenter le niveau pour l'indentation
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("right : %d\n", and->right->type);
        printer(and->right, and->type, level + 1); // Augmenter le niveau pour l'indentation
    }
    else if (cmd->type == 6) {
        or = (t_orcmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("OR\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("left : %d\n", or->left->type);
        printer(or->left, or->type, level + 1); // Augmenter le niveau pour l'indentation
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("right : %d\n", or->right->type);
        printer(or->right, or->type, level + 1); // Augmenter le niveau pour l'indentation
    }
    if (cmd->type == 8) 
    {
        ex = (t_execcmd *)cmd;
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("EMPTY\n");
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        printf("args[0] : %s\n", ex->args[0]);
    } 
}
// int main(int ac, char **av)
// {
//     printer(parsecmd(av[1]), 0, 0);
//     // parsecmd(av[1]);
// }