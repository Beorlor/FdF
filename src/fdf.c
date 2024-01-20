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

	print_point_list(map.points);

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
	float scale = 30.0;
	t_point translate = {100, 100, 0, 0};

	// Render the grid
	render_grid(&map, img, scale, translate);

    // Render the image to the window
    render(mlx, img);

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

