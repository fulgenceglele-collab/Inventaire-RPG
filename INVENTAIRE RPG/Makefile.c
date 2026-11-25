
NAME	= inventaire_rpg
CC		= gcc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= ft_lstnew.c ft_lstadd_back.c ft_lstsize.c ft_lstdelone.c \
		  ft_lstclear.c ft_lstiter.c main.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c list.h objet.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
