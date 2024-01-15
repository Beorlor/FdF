#ifndef FDF_H
# define FDF_H

# include "../libft/libft.h"
# include "../gnl/get_next_line.h"
# include "mlx.h"
# include <X11/keysym.h>
# include <X11/X.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>

typedef struct s_mlx {
    void    *mlx_ptr;
    void    *win_ptr;
}               t_mlx;

typedef struct  s_point {
    int     x;
    int     y;
    int     z;
}               t_point;

typedef struct  s_point_list {
    t_point         point;
    struct s_point_list   *next;
}               t_point_list;

// fdf
void print_point_list(t_point_list *list);
t_mlx   *init_mlx(int width, int height, char *title);

// parsing
bool    parse_file(char *filename, t_point_list **list);
void    add_point_to_list(t_point_list **list, int x, int y, int z);
bool is_line_valid(const char *line);
bool is_valid_integer(const char *str);

#endif
