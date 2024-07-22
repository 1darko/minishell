/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakojic <dakojic@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 14:03:36 by dakojic           #+#    #+#             */
/*   Updated: 2024/07/22 16:41:40 by dakojic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define EXEC 1
#define REDIR 2
#define PIPE 3
#define BLOC 4
#define AND 5
#define OR 6
#define HERE 7
#define EMPTY 8
#define MAXARGS 4096
#define MAXLINE 1500001



// Structs 
typedef struct s_cmd{
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
int ft_strcmp(const char *s1, const char *s2);
int ft_echo(t_execcmd **cmd);

#endif