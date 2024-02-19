#include "../include/fdf.h"

int ft_error(char *message)
{
    write(2, message, ft_strlen(message));
    write(2, "\n", 1);
    return (EXIT_FAILURE);
}

int	exit_cleanup(void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	if (fdf->map != NULL)
	{
		free_point_list(&(fdf->map->points));
	}
	if (fdf->img != NULL)
	{
		mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
		free(fdf->img);
	}
	if (fdf->mlx->win_ptr != NULL)
	{
		mlx_destroy_window(fdf->mlx->mlx_ptr, fdf->mlx->win_ptr);
	}
	if (fdf->mlx != NULL)
	{
		free(fdf->mlx);
	}
	exit(EXIT_SUCCESS);
}

// Choose the smaller scale factor to ensure the entire map fits within the window
int	calculate_initial_scale(int num_cols, int num_rows)
{
	const int	max_cols_for_default_scale = 25;
	const int	max_rows_for_default_scale = 20;
	const int	default_scale = 30;
	int			scale_factor_col;
	int			scale_factor_row;
	int			scale_factor;

	scale_factor_col = (num_cols > max_cols_for_default_scale) ? (max_cols_for_default_scale
			* 100) / num_cols : 100;
	scale_factor_row = (num_rows > max_rows_for_default_scale) ? (max_rows_for_default_scale
			* 100) / num_rows : 100;
	scale_factor = (scale_factor_col < scale_factor_row) ? scale_factor_col : scale_factor_row;
	return ((default_scale * scale_factor) / 100);
}

static void	init_fdf(t_fdf *fdf, t_map *map, t_mlx *mlx, t_img *img)
{
	fdf->map = map;
	fdf->mlx = mlx;
	fdf->img = img;
	fdf->scale = calculate_initial_scale(map->num_cols, map->num_rows);
	fdf->translate.x = 500;
	fdf->translate.y = 150;
	fdf->translate.z = 0;
	fdf->rotation.x = 0;
	fdf->rotation.y = 0;
	fdf->rotation.z = 0;
	fdf->is_isometric = true;
}

static void	cleanup(t_fdf *fdf)
{
	if (fdf->map)
		free_point_list(&fdf->map->points);
	if (fdf->img)
	{
		mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
		free(fdf->img);
	}
	if (fdf->mlx)
	{
		if (fdf->mlx->win_ptr)
			mlx_destroy_window(fdf->mlx->mlx_ptr, fdf->mlx->win_ptr);
		free(fdf->mlx);
	}
}

static void	setup_hooks(t_mlx *mlx, t_fdf *fdf)
{
	mlx_key_hook(mlx->win_ptr, handle_key, fdf);
	mlx_hook(mlx->win_ptr, 17, 0, exit_cleanup, fdf);
	mlx_loop(mlx->mlx_ptr);
}

static void	render_setup(t_fdf *fdf)
{
	if (fdf->is_isometric)
		render_iso(fdf);
	else
		render_grid(fdf);
	render(fdf->mlx, fdf->img);
}

int	main(int argc, char **argv)
{
	t_mlx	*mlx;
	t_img	*img;
	t_map	map;
	t_fdf	fdf;

	if (argc != 2)
		return ft_error("Usage: ./fdf filename");
	if (!parse_file(argv[1], &map))
		return EXIT_FAILURE;
	mlx = init_mlx(1280, 720, "FdF");
	if (!mlx)
		return cleanup(&fdf), EXIT_FAILURE;
	img = init_img(mlx, 1280, 720);
	if (!img)
		return cleanup(&fdf), EXIT_FAILURE;
	init_fdf(&fdf, &map, mlx, img);
	render_setup(&fdf);
	setup_hooks(mlx, &fdf);
	cleanup(&fdf);
	return (EXIT_SUCCESS);
}

// Adjustments for handle_key function according to 42 norms
static void handle_movement(int keycode, t_fdf *fdf)
{
    if (keycode == KEY_W)
        fdf->translate.y -= 10;
    else if (keycode == KEY_A)
        fdf->translate.x -= 10;
    else if (keycode == KEY_S)
        fdf->translate.y += 10;
    else if (keycode == KEY_D)
        fdf->translate.x += 10;
}

static void handle_scale(int keycode, t_fdf *fdf)
{
    if (keycode == KEY_Q)
        fdf->scale *= 0.9;
    else if (keycode == KEY_E)
        fdf->scale *= 1.1;
}

static void handle_rotation(int keycode, t_fdf *fdf)
{
    if (keycode == KEY_Y)
        fdf->rotation.x += 1;
    else if (keycode == KEY_H)
        fdf->rotation.x -= 1;
    else if (keycode == KEY_U)
        fdf->rotation.y += 1;
    else if (keycode == KEY_J)
        fdf->rotation.y -= 1;
    else if (keycode == KEY_I)
        fdf->rotation.z += 1;
    else if (keycode == KEY_K)
        fdf->rotation.z -= 1;
}

static void handle_projection_mode(int keycode, t_fdf *fdf)
{
    if (keycode == KEY_O)
        fdf->is_isometric = !fdf->is_isometric;
}

static void reinit_image(t_fdf *fdf)
{
    if (fdf->img != NULL)
    {
        mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
        free(fdf->img);
    }
    fdf->img = init_img(fdf->mlx, 1280, 720);
    render_setup(fdf);
}

int handle_key(int keycode, t_fdf *fdf)
{
    handle_movement(keycode, fdf);
    handle_scale(keycode, fdf);
    handle_rotation(keycode, fdf);
    handle_projection_mode(keycode, fdf);
    if (keycode == KEY_ESC)
    {
        exit_cleanup(fdf);
        return (0);
    }
    reinit_image(fdf);
    return (0);
}
