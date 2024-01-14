NAME = fdf

CC = gcc

CFLAGS = -Werror -Wall -Wextra -fsanitize=address

RM = rm -rf

# Add your FdF source files here
SRCS =	src/fdf.c \
		src/other_sources.c \
		src/more_sources.c \
		libft/libft.a \
		gnl/libgnl.a

LIBFT = libft/libft.a
LIBGNL = gnl/libgnl.a

# Include MiniLibX flags if necessary
MLX_FLAGS = -L /path/to/minilibx -lmlx -framework OpenGL -framework AppKit

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
