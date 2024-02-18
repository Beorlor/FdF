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
# include <math.h>

# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

typedef struct s_point {
    float x;
    float y;
    float z;
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

typedef struct s_fdf {
    t_map *map;
    t_mlx *mlx;
    t_img *img;
    float scale;
    t_point translate;
	t_point rotation;
	bool is_isometric;
} t_fdf;

// fdf
void print_point_list(t_point_list *list);
int calculate_initial_scale(int num_cols, int num_rows);
int handle_key(int keycode, t_fdf *fdf);
int exit_cleanup(void *param);

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
void put_pixel_to_img(t_img *img, float x, float y, int color);
void render(t_mlx *mlx, t_img *img);

// basic_projection
t_point orthogonal_project_point(t_point point3D, float scale, t_point translate);
void render_grid(t_map *map, t_img *img, float scale, t_point translate);
t_point get_point_at(t_map *map, float x, float y);
void bresenham_draw_line(t_img *img, t_point p0, t_point p1);
int determine_color(int height);

// isometric projection
t_point isometric_project_point(t_point point3D, float scale, t_point translate, t_point rotation);
void render_iso(t_fdf *fdf);
t_point rotate_x(t_point p, float angle);
t_point rotate_y(t_point p, float angle);
t_point rotate_z(t_point p, float angle);

#endif
