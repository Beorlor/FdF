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

	// Example values for scale and translation
	t_fdf fdf = {&map, mlx, img, 30.0, {100, 100, 0, 0}};

	// Render the grid
	render_grid(&map, img, fdf.scale, fdf.translate);

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

// Key handling function

int handle_key(int keycode, t_fdf *fdf) {
    const int KEY_W = 119; // Move up
    const int KEY_A = 97;  // Move left
    const int KEY_S = 115; // Move down
    const int KEY_D = 100; // Move right
    const int KEY_Q = 113; // Scale down
    const int KEY_E = 101; // Scale up
    const int KEY_ESC = 65307; // Exit

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

    if (fdf->img != NULL) {
        mlx_destroy_image(fdf->mlx->mlx_ptr, fdf->img->img_ptr);
        free(fdf->img);
    }

    fdf->img = init_img(fdf->mlx, 1280, 720);
    render_grid(fdf->map, fdf->img, fdf->scale, fdf->translate);
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
    exit(0);
}


