
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
#include "minishell.h"





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


void nulargs(t_cmd* cmd)
{
    int i;
    int j;

    i = 0;
    t_execcmd *exe;

    exe = (t_execcmd *)cmd;

    while((exe->args[i]) != 0)
    {
        *exe->args[i] = 0;
        i++;
    }
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
    return (0);
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


// t_herepipe  ft_lastpipe(t_herepipe *pipe)
// {
//     t_herepipe *temp;

//     temp = pipe;
//     while(temp->next)
//         temp = temp->next;
//     return (*temp);
// }










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
    if(exec->args == NULL)
    {
        printf("Exec NULL\n");
        return ;
    }
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

// void free_herepipe(t_herepipe *pipe)
// {
//     t_herepipe *temp;

//     while (pipe)
//     {
//         temp = pipe->next;
//         free(pipe->str);
//         free(pipe);
//         pipe = temp;
//     }
// }

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
    free_herepipe(&shell);  // Free the here-doc pipes
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