NAME = fdf

CC = gcc
CFLAGS = -Werror -Wall -Wextra -fsanitize=address
RM = rm -rf

OBJ_DIR = obj/
SRC_DIR = src/

# Add your FdF source files here
SRCS = fdf.c \
       parsing/parsing.c \
       minilibx_management/image.c \
       drawing/basic_projection.c \
	   drawing/isometric_projection.c \

# Create object file paths in the obj directory
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))

LIBFT = libft/libft.a
LIBGNL = gnl/libgnl.a

MLX_FLAGS = -L minilibx-linux -lmlx -lXext -lX11 -lm

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(LIBGNL)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBGNL) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)parsing/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)minilibx_management/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(SRC_DIR)drawing/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft

$(LIBGNL):
	make -C gnl

clean:
	$(RM) $(OBJ_DIR)
	make clean -C libft
	make clean -C gnl

fclean: clean
	$(RM) $(NAME)
	make fclean -C libft
	make fclean -C gnl

re: fclean all

.PHONY: all clean fclean re
