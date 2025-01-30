CC				= cc
RM				= rm -rf 
CFLAGS 			= -Wall -Werror -Wextra -g
LIB 			= -L./libft -lft

SRC 			= children.c pipex.c utils.c
OBJ 			= $(SRC:.c=.o)

LIBFT_DIR 		= ./libft
LIBFT			= $(LIBFT_DIR)/libft.a

NAME			= pipex

all: $(NAME)

$(NAME): $(OBJ) libft
	$(CC) $(OBJ) $(LIB) -o$(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

all:			$(NAME)

clean:
				$(RM) $(OBJ) 
				$(MAKE) clean -C $(LIBFT_DIR)

fclean:			clean
				$(RM) $(NAME) $(LIBFT)

re:				fclean $(NAME)

.PHONY:			libft all clean fclean re 
