NAME = fdf

CC = gcc

CFLAGS = -Werror -Wall -Wextra -fsanitize=address

RM = rm -rf

# Add your FdF source files here
SRCS =	src/fdf.c \
		libft/libft.a \
		gnl/libgnl.a

LIBFT = libft/libft.a
LIBGNL = gnl/libgnl.a

MLX_FLAGS = -L minilibx-linux -lmlx -lXext -lX11

all: $(NAME)

$(NAME): $(LIBFT) $(LIBGNL)
	$(CC) $(CFLAGS) $(SRCS) $(MLX_FLAGS) -o $(NAME)

$(LIBFT):
	make -C libft

$(LIBGNL):
	make -C gnl

clean:
	$(RM) $(NAME)
	make clean -C libft
	make clean -C gnl

fclean: clean
	make fclean -C libft
	make fclean -C gnl

re: fclean all

.PHONY: all clean fclean re
