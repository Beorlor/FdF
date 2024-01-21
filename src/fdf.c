#include "../include/fdf.h"

int main(int argc, char **argv) {
    t_mlx *mlx;
    t_img *img;
	t_map map;

    if (argc != 2) {
        ft_putendl_fd("Usage: ./fdf filename", 2);
        return (EXIT_FAILURE);
    }

    if (!parse_file(argv[1], &map)) {
        return (EXIT_FAILURE);
    }

	//print_point_list(map.points);

    // Initialize MiniLibX
    mlx = init_mlx(1280, 720, "FdF");
    if (!mlx) {
        // Free the point list before exiting
        free_point_list(&map.points);
        return (EXIT_FAILURE);
    }

    // Initialize the image
    img = init_img(mlx, 1280, 720);
    if (!img) {
        free(mlx);
        free_point_list(&map.points);
        return (EXIT_FAILURE);
    }

	int initial_scale = calculate_initial_scale(map.num_cols, map.num_rows);
	t_fdf fdf = {&map, mlx, img, initial_scale, {0, 0, 0, 0}, {0, 0, 0, 0}, true};

	// Render the grid
	render_iso(&map, img, fdf.scale, fdf.translate, fdf.rotation);

    // Render the image to the window
    render(mlx, img);

	mlx_key_hook(mlx->win_ptr, handle_key, &fdf);

    // Start the MiniLibX loop
    mlx_loop(mlx->mlx_ptr);

    // Clean up
    free_point_list(&map.points);
    mlx_destroy_image(mlx->mlx_ptr, img->img_ptr);
    free(img);
    free(mlx);

    return (EXIT_SUCCESS);
}

void print_point_list(t_point_list *list) {
    while (list != NULL) {
        printf("Point (x: %d, y: %d, z: %d, color: %d)\n", list->point.x, list->point.y, list->point.z, list->point.color);
        list = list->next;
    }
}

int calculate_initial_scale(int num_cols, int num_rows) {
    const int max_cols_for_default_scale = 25;
    const int max_rows_for_default_scale = 20;
    const int default_scale = 30;

    int scale_factor_col = (num_cols > max_cols_for_default_scale) ? (max_cols_for_default_scale * 100) / num_cols : 100;
    int scale_factor_row = (num_rows > max_rows_for_default_scale) ? (max_rows_for_default_scale * 100) / num_rows : 100;

    // Choose the smaller scale factor to ensure the entire map fits within the window
    int scale_factor = (scale_factor_col < scale_factor_row) ? scale_factor_col : scale_factor_row;

    return (default_scale * scale_factor) / 100;
}

// Key handling function
int handle_key(int keycode, t_fdf *fdf) {
    const int KEY_W = 119; // Move up
    const int KEY_A = 97;  // Move left
    const int KEY_S = 115; // Move down
    const int KEY_D = 100; // Move right
    const int KEY_Q = 113; // Scale down
    const int KEY_E = 101; // Scale up
    const int KEY_ESC = 65307; // Exit
	const int KEY_O = 111;  // Toggle Projection Mode
	const int KEY_Y = 121;  // Rotate +X
	const int KEY_H = 104;  // Rotate -X
	const int KEY_U = 117;  // Rotate +Y
	const int KEY_J = 106;  // Rotate -Y
	const int KEY_I = 105;  // Rotate +Z
	const int KEY_K = 107;  // Rotate -Z

    if (keycode == KEY_W) {
        fdf->translate.y -= 10;
    } else if (keycode == KEY_A) {
        fdf->translate.x -= 10;
    } else if (keycode == KEY_S) {
        fdf->translate.y += 10;
    } else if (keycode == KEY_D) {
        fdf->translate.x += 10;
    } else if (keycode == KEY_Q) {
        fdf->scale *= 0.9;
    } else if (keycode == KEY_E) {
        fdf->scale *= 1.1;
    } else if (keycode == KEY_ESC) {
        exit_cleanup(fdf);
    }

    // Projection Mode Toggle
    if (keycode == KEY_O) {
        fdf->is_isometric = !fdf->is_isometric; // Toggle projection mode
    }

    // Rotation Handling
    if (keycode == KEY_Y) fdf->rotation.x += 5;
    if (keycode == KEY_H) fdf->rotation.x -= 5;
    if (keycode == KEY_U) fdf->rotation.y += 5;
    if (keycode == KEY_J) fdf->rotation.y -= 5;
    if (keycode == KEY_I) fdf->rotation.z += 5;
    if (keycode == KEY_K) fdf->rotation.z -= 5;

    // Redraw the image
    if (fdf->img != NULL) {
        mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
        free(fdf->img);
    }

    fdf->img = init_img(fdf->mlx, 1280, 720);

    // Render based on current projection mode
    if (fdf->is_isometric) {
        render_iso(fdf->map, fdf->img, fdf->scale, fdf->translate, fdf->rotation);
    } else {
        render_grid(fdf->map, fdf->img, fdf->scale, fdf->translate); // Orthogonal projection
    }

    render(fdf->mlx, fdf->img);

    return 0;
}


void exit_cleanup(t_fdf *fdf) {
    if (fdf->map != NULL) {
        free_point_list(&(fdf->map->points));
    }
    if (fdf->img != NULL) {
        mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
        free(fdf->img);
    }
    if (fdf->mlx->win_ptr != NULL) {
        mlx_destroy_window(fdf->mlx->mlx_ptr, fdf->mlx->win_ptr);
    }
    if (fdf->mlx != NULL) {
        free(fdf->mlx);
    }
    exit(EXIT_SUCCESS);
}
