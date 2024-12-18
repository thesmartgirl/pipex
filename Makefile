CC				= cc
RM				= rm -rf *.o ./*/*.o
CFLAGS 			= -Wall -Werror -Wextra
LIB 			= -L./mlx -lmlx -lXext -lX11 -L./libft -lft

SRC 			= 
OBJ 			= $(SRC:.c=.o)

LIBFT_DIR 		= ./libft
LIBFT			= $(LIBFT_DIR)/libft.a

NAME			= pipex

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(OBJ) $(LIB) -o$(NAME)

$(LIBFT):
	$(MAKE) $(LIBFT_DIR)

all:			$(NAME)

clean:
				$(RM) $(OBJS) $(BONUS_OBJS)

fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY:			all clean fclean re 
