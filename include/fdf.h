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

# define MAX_COLS_FOR_DEFAULT_SCALE 25
# define MAX_ROWS_FOR_DEFAULT_SCALE 20
# define DEFAULT_SCALE 30

# define BLUE 0x0000FF
# define DOGER_BLUE 0x1E90FF
# define CYAN 0x00FFFF
# define GREEN 0x00FA9A
# define LIME_GREEN 0x32CD32
# define YELLOW 0xFFFF00
# define GOLD 0xFFD700
# define ORANGE 0xFFA500
# define ORANGE_RED 0xFF4500
# define RED 0xFF0000
# define BLANK 0xFFFFFF

// Key definitions for movement
# define KEY_W 119       // Move up
# define KEY_A 97        // Move left
# define KEY_S 115       // Move down
# define KEY_D 100       // Move right

// Key definitions for scaling
# define KEY_Q 113       // Scale down
# define KEY_E 101       // Scale up

// Key definition for exit
# define KEY_ESC 65307   // Exit

// Key definitions for projection mode and rotation
# define KEY_O 111       // Toggle Projection Mode
# define KEY_Y 121       // Rotate +X
# define KEY_H 104       // Rotate -X
# define KEY_U 117       // Rotate +Y
# define KEY_J 106       // Rotate -Y
# define KEY_I 105       // Rotate +Z
# define KEY_K 107       // Rotate -Z

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

// struct for brasenham algo to be complient with 42 norm
typedef struct s_draw
{
	int	startx;
	int	starty;
	int	endx;
	int	endy;
	int	deltax;
	int	deltay;
	int	stepx;
	int	stepy;
	int	error;
	float	line_length;
	float	height_diff;
	float	height_step;
	float	current_height;
}			t_draw;

// main.c
int	main(int argc, char **argv);
void	init_fdf(t_fdf *fdf, t_map *map, t_mlx *mlx, t_img *img);
void	render_setup(t_fdf *fdf);
void	cleanup(t_fdf *fdf);
void	setup_hooks(t_mlx *mlx, t_fdf *fdf);

// brasenham.c
void	bresenham_draw_line(t_img *img, t_point p0, t_point p1);
void	init_line_drawing(t_draw *draw, t_point p0, t_point p1);
void	draw_line_loop(t_img *img, t_draw *draw, t_point p0);
int	get_line_color(t_point p0, float current_height);
void	update_line_drawing(t_draw *draw);

// drawing_utilities.c
void	put_pixel_to_img(t_img *img, float x, float y, int color);
t_point	get_point_at(t_map *map, float x, float y);
int	determine_color(int height);

// isometric_projection.c
t_point	rotate_x(t_point p, float angle);
t_point	rotate_y(t_point p, float angle);
t_point	rotate_z(t_point p, float angle);
void	render_iso_lines(t_fdf *fdf, int x, int y);

// projections_and_rendering.c
t_point	orthogonal_project_point(t_point point3d, float scale, t_point translate);
t_point	isometric_project_point(t_point point3d, float scale, t_point translate, t_point rotation);
void	render_grid(t_fdf *fdf);
void	render_iso(t_fdf *fdf);
void	draw_line_conditions(t_fdf *fdf, int x, int y);

// key_handling.c
int	handle_key(int keycode, t_fdf *fdf);
void	handle_movement(int keycode, t_fdf *fdf);
void	handle_scale(int keycode, t_fdf *fdf);
void	handle_rotation(int keycode, t_fdf *fdf);
void	handle_projection_mode(int keycode, t_fdf *fdf);

// mlx_management.c
t_mlx	*init_mlx(int width, int height, char *title);
t_img	*init_img(t_mlx *mlx, int width, int height);
void	render(t_mlx *mlx, t_img *img);

// free_parsing.c
void	free_token(char **token);
void	free_tokens(char **tokens);
void	free_single_point_list(t_point_list **list);
void	free_point_list(t_point_list **list);

// parsing.c
bool	parse_file(char *filename, t_map *map);
void	init_map(t_map *map);
bool	read_and_process_line(int fd, t_map *map, int *y, int *num_cols);
void	add_point_to_list(t_point_list **list, t_point point);

// parsing_utils.c
int	open_file(char *filename);
bool	check_file_descriptor(int fd);
bool	validate_num_cols(int *num_cols, int x);
bool	add_point(char **point_data, t_map *map, int x, int y);
bool	process_tokens(char **tokens, t_map *map, int y, int *num_cols);

// validation.c
bool	is_hex_digit(char c);
bool	is_valid_hex_color(const char *str);
bool	is_valid_integer(const char *str);
bool	is_line_valid(const char *line);

// exit.c
int	exit_cleanup(void *param);
int	ft_error(char *message);

// utils.c
float	min(float a, float b);
float	max(float a, float b);
int	calculate_scale_factor(int size, int max_size);
int	calculate_initial_scale(int num_cols, int num_rows);
void	reinit_image(t_fdf *fdf);
#endif
