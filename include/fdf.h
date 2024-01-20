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

typedef struct s_img {
    void    *img_ptr;
    char    *addr;
    int     bpp;
    int     line_length;
    int     endian;
}               t_img;

typedef struct s_point {
    int x;
    int y;
    int z;
    int color; // Color information
} t_point;


typedef struct  s_point_list {
    t_point         point;
    struct s_point_list   *next;
}               t_point_list;

typedef struct s_map {
    t_point_list *points;
    int num_rows;
    int num_cols;
} t_map;


// fdf
void print_point_list(t_point_list *list);

// parsing
void free_point_list(t_point_list **list);
bool parse_file(char *filename, t_map *map);
void add_point_to_list(t_point_list **list, int x, int y, int z, int color);
bool is_line_valid(const char *line);
bool is_valid_hex_color(const char *str);
int ft_isxdigit(int c);
bool is_valid_integer(const char *str);

// image
t_mlx   *init_mlx(int width, int height, char *title);
t_img *init_img(t_mlx *mlx, int width, int height);
void put_pixel_to_img(t_img *img, int x, int y, int color);
void render(t_mlx *mlx, t_img *img);

// basic_projection
t_point orthogonal_project_point(t_point point3D, float scale, t_point translate);
void render_grid(t_map *map, t_img *img, float scale, t_point translate);
t_point get_point_at(t_map *map, int x, int y);
void draw_line(t_img *img, t_point p0, t_point p1);
int determine_color(float height);

#endif
