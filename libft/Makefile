NAME = libft.a
SOURCES = ft_bzero.c ft_atoi.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
	ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
	ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
	ft_putendl_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_split.c \
	ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
	ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
	ft_strtrim.c ft_strrchr.c ft_substr.c ft_tolower.c ft_toupper.c
BSOURCES = ft_lstadd_back.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c \
	ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c 
OBJECTS = $(SOURCES:.c=.o)
BOBJECTS = $(BSOURCES:.c=.o)
AR = ar rcs

CC = cc
CFLAGS = -Wall -Wextra -Werror


# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o $@ $<

.c.o: $(SOURCES) $(BSOURCES)
	$(CC) $(CFLAGS) -c -o $@ $^

all: $(NAME)
	@echo " /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < "
	@echo " /\_/\   ╦  ╦╔╗ ╔═╗╔╦╗  ╦═╗╔═╗╔═╗╔╦╗╦ ╦    /\_/\ "
	@echo "( o.o )  ║  ║╠╩╗╠╣  ║   ╠╦╝║╣ ╠═╣ ║║╚╦╝   ( o.o )"
	@echo " > ^ <   ╩═╝╩╚═╝╚   ╩   ╩╚═╚═╝╩ ╩═╩╝ ╩     > ^ < "
	@echo " /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < "

$(NAME): $(OBJECTS)
	$(AR) $@ $^

bonus: $(OBJECTS) $(BOBJECTS)
	$(AR) $(NAME) $^
	@echo " /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < "
	@echo " /\_/\  ╦  ╦╔╗ ╔═╗╔╦╗  ╔╗ ╔═╗╔╗╔╦ ╦╔═╗  ╦═╗╔═╗╔═╗╔╦╗╦ ╦  /\_/\ "
	@echo "( o.o ) ║  ║╠╩╗╠╣  ║   ╠╩╗║ ║║║║║ ║╚═╗  ╠╦╝║╣ ╠═╣ ║║╚╦╝ ( o.o )"
	@echo " > ^ <  ╩═╝╩╚═╝╚   ╩   ╚═╝╚═╝╝╚╝╚═╝╚═╝  ╩╚═╚═╝╩ ╩═╩╝ ╩   > ^ < "
	@echo " /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\  /\_/\ "
	@echo "( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )( o.o )"
	@echo " > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ <  > ^ < "

clean:
	rm -f $(OBJECTS) $(BOBJECTS)

fclean: clean
	rm -f $(NAME) libft.so
	@echo " ___/-\___"
	@echo "|--TRASH--|"
	@echo " | | | | |"
	@echo " | | | | |"
	@echo " | | | | |"
	@echo " | | | | |"
	@echo " |_LIBFT_|"
	@echo " |_______|"

re: fclean all

.PHONY: all bonus clean fclean re