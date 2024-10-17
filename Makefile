NAME = minish

FLAGS = -g -MMD -Wall -Werror -Wextra -g3 -lreadline

CC = cc

BUILT = builtin/

RM = rm -rf 

SRC = ft_free.c lexing_check.c tools_heredoc.c parse_redirs.c \
	ft_memory.c ft_split.c ft_free2.c init_structs.c \
	tools_parsing.c parsing_heredoc.c lexer.c init_redirs_er.c \
	parsing_exec.c libft.c sub_lexer.c \
	init_redirs.c ft_parsing.c tools_node.c tokens.c \
	minishell.c ft_memory2.c lexer_checks.c misc.c \
	quotes.c $(BUILT)ft_cd.c moded_libft.c $(BUILT)ft_env.c \
	$(BUILT)ft_pwd.c $(BUILT)ft_echo.c $(BUILT)ft_exit.c \
	$(BUILT)ft_unset.c $(BUILT)ft_export.c execfree.c

all: $(NAME)

DEP = $(SRC:.c=.d)

OBJ = $(SRC:.c=.o)

$(NAME) : $(OBJ)
		$(CC) $(FLAGS)  -o $@ $^

#-include $(DEP)

#%.o: %.c
#	$(CC) $(FLAGS) -c $< -o $@


clean:
	$(RM) $(OBJ)

fclean: clean
		$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
