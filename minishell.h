/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:03:36 by dakojic           #+#    #+#             */
/*   Updated: 2024/10/17 11:06:41 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
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
#include <stdbool.h>

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



typedef struct s_fds
{
	int					fd;
	int					type;
}						t_fds;




typedef struct s_cmd
{
    int type;
}   t_cmd;



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
    int quote;
    struct s_herepipe *next;
} t_herepipe;

typedef struct s_shell
{
    int type;
    t_cmd *tree;
    t_var *var;
    char **env;
    t_herepipe *pipe;
}   t_shell;

typedef struct s_execs
{
	t_fds				*fds;
	t_shell				*shell;
	int					ret;
	t_cmd				*cmd;
}						t_execs;

typedef struct s_execcmd
{
    int type;
    char **args;
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
    int quote;
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

typedef struct s_lexer
{
    int type;
    char *heredoc;
    struct s_lexer *next;
    struct s_lexer *prev;
}   t_lexer;




// int type_check(t_lexer *lex, int *i, t_shell **shell);
// t_cmd *redircmd_here2(t_herepipe **pipe, t_cmd *cmd);
// t_cmd *redircmd_here(t_herepipe **pipe, t_cmd *cmd);
// void findredir(t_cmd *cmd, t_cmd **temp);
// int init_heredoc(t_lexer *lex, t_shell **shell);
// t_cmd *parse_or(t_shell *shell, char **ptr);
// t_cmd *parse_and(t_shell *shell, char **ptr);
// t_cmd *parseline(t_shell *shell, char **ptr);
// void nulterminator(t_cmd *cmd);
// void nulargs(t_cmd *cmd);
// void nulredir(t_cmd *cmd);
// void ft_bzero(void *s, size_t n);
// int ft_strlen(const char *s);
// char *ft_strchr(char *s, char c);
// int lfsymbol(char **ptr, char *c);
// t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd);
// t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd);
// t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd);
// t_cmd *execcmd(void);
// t_cmd *pipecmd(t_cmd *left, t_cmd *right);
// t_cmd *parseredirs_er(t_herepipe **pipes, t_cmd *cmd, char **ps);
// void    parseredirs_primo(t_herepipe **pipes, t_cmd **cmd, char **ps, int *check);
// int gettoken(char **ptr, char **ptr_token, char **ptr_endtoken);
// void problem(char *s);
// t_cmd *parseblock(t_shell *shell, char **ps,  int *check);
// t_cmd *parseexec(t_shell *shell, char **ps);
// t_cmd *parse_double_node(t_shell *shell, char **ptr);
// void *parsecmd(t_shell **shell, char *str);
// t_cmd *parse_all_redirections(t_cmd *cmd, char **ps);
// int parse_single_argument(t_execcmd *cmd, char **ps, int argc);
// void initialize_cmd(t_cmd **ret, t_execcmd **cmd, int *argc);
// int lexing_check(t_shell **shell ,t_lexer *lexer);
// int lexer(t_shell **shell, char *str);
// void tree_free(t_cmd **tree);
// // Pipex 
// void ft_exec(t_cmd *tree, char **env);
// char	*get_path(char *cmd, char **env);
// static char	*ft_strjoin(char const *s1, char const *s2);
// char	*get_env(char *name, char **env);
// static char	*ft_substr(char const *s, unsigned int start, size_t len);
// void	free_array(char **s);
// int sub_lexer(t_lexer **lex, char *str, int *i);
// void free_lexer(t_lexer *lexer);
// size_t	ft_strlcpy(char *dst, const char *src, size_t size);

// Proto

// Free

void	free_array(char **s);
void free_args(t_execcmd *ex);
void free_exec(t_execcmd *ex);
void free_sub(t_sub *sub);
void  free_redir(t_redircmd *re);
void free_double(t_doublecmd *db);
void tree_free(t_cmd **tree);
void free_herepipe(t_shell **shell);

//Libfte
char	*ft_strdup(const char *s);
void	*ft_memset(void *s, int c, size_t n);
int	ft_strcmp(const char *s1, const char *s2);
void	*ft_calloc(size_t nmemb, size_t size);
char *ft_strchr(char *s, char c);
void ft_bzero(void *s, size_t n);
int ft_strlen(const char *s);
char	**ft_split(char const *str, char c);
void	*ft_calloc(size_t nmemb, size_t size);
int lfsymbol(char **ptr, char *c);
char *ft_filecpy(char *file, char *efile);
char	*ft_strdup(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	ft_bzero(void *s, size_t n);
// List functions

t_lexer	*ft_lstlast(t_lexer *lst);
void	ft_lstadd_back(t_lexer **lst, t_lexer **new);



//Libft moded
char **args_malloc(int argc, char *ptr_arg, char *ptr_earg, char **args);
char *empty_string(); // calloc 1 1 ?? 
void ft_strcpy_quoteless(char **dest, char *src);


// CMD init

t_cmd *redircmd_out(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_in(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_append(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_in2(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_out2(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_append2(t_cmd *cmd, char *file, char *efile, int fd);
t_cmd *redircmd_here(t_herepipe **pipe, t_cmd *cmd);
t_cmd *redircmd_here2(t_herepipe **pipe, t_cmd *cmd);
t_cmd *execcmd(void);
t_cmd *pipecmd(t_cmd *left, t_cmd *right);
t_cmd *andcmd(t_cmd *left, t_cmd *right);
t_cmd *orcmd(t_cmd *left, t_cmd *right);


// Parsing

void    parseredirs_primo(t_herepipe **pipes, t_cmd **cmd, char **ps, int *check);
t_cmd *parseredirs_er(t_herepipe **pipes, t_cmd *cmd, char **ps);
void findredir(t_cmd *cmd, t_cmd **temp);

char *other_token(char *temp, int *check);
void    extratoken(char **temp, int *check);
int gettoken(char **ptr, char **ptr_token, char **ptr_endtoken);
//
t_cmd *parseexec(t_shell *shell, char **ps, int check, int argc) ;
t_cmd *parse_double_node(t_shell *shell, char **ptr);
t_cmd *parse_and(t_shell *shell, char **ptr);
t_cmd *parse_or(t_shell *shell, char **ptr);
t_cmd *parseline(t_shell *shell, char **ptr);
void *parsecmd(t_shell **shell, char *str);
int quote_check(char *str);

// Lexer
int lexer(t_shell **shell, char *str);
int sub_lexer(t_lexer **lex, char *str, int *i);
int prev_check(t_lexer *lex, char *str);
int next_check(t_lexer *lex, char *str);
int first_type_check(t_lexer *lex, int *i, t_shell **shell);
char *heredoc_filler(char *end);
int init_heredoc(t_lexer *lex, t_shell **shell);
int lexing_check(t_shell **shell, t_lexer *lexer);
void free_lexer(t_lexer *lexer);

int add_quote_node(t_lexer **lex, char *str, int *i, char c);
void skip_quotes(char *str, int *i, int *size);
void  ft_pipeaddback(t_shell **shell, t_herepipe *new);

// Useless ???
void nulargs(t_cmd* cmd);
void nulredir(t_cmd *cmd);
void nulterminator(t_cmd *cmd);
void emptyswitch(t_cmd *cmd);

// Printer

void printer(t_cmd *cmd, int s, int level);
void print_indent(int indent);
void print_redircmd(t_redircmd *redir, int indent);
void print_execcmd(t_execcmd *exec, int indent);
void print_pipecmd(t_pipecmd *pipe, int indent);
void print_andcmd(t_andcmd *and, int indent);
void print_doublecmd(t_doublecmd *dbl, int indent);
void print_orcmd(t_orcmd *or, int indent);
void print_subcmd(t_sub *sub, int indent);
void print_cmd(t_cmd *cmd, int indent);

// To remove ?? 
void free_var(t_var *var);
void free_env(char **env);
void free_cmd(t_cmd *cmd);
void free_shell(t_shell *shell);


#endif